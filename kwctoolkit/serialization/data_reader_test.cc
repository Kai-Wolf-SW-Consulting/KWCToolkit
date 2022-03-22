// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/serialization/data_reader.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstring>
#include <memory>
#include <string>
#include <type_traits>

#include "kwctoolkit/base/callback.h"
#include "kwctoolkit/base/integral_types.h"
#include "kwctoolkit/base/status.h"

using namespace kwc;
using kwc::Callback;
using kwc::base::Status;
using kwc::serialization::DataReader;

using testing::_;
using testing::DoAll;
using testing::Invoke;
using testing::InvokeWithoutArgs;
using testing::Return;

namespace {
using ReadCallback = kwc::Callback2<int64, char*>;

void Raise(bool* called) {
    *called = true;
}

void ReadNextCharToBuffer(const char** from, int64 count, char* buffer) {
    EXPECT_EQ(1, count);
    if (*from) {
        *buffer = **from;
        ++*from;
    }
}
}  // namespace

class MockDataReader : public DataReader {
  public:
    MockDataReader() : DataReader(nullptr) {}
    explicit MockDataReader(Callback* delete_cb) : DataReader(delete_cb) {}
    ~MockDataReader() override = default;

    MOCK_METHOD1(doSetOffset, int64(int64 offset));
    MOCK_METHOD2(doReadIntoBuffer, int64(int64 max_bytes, char* append_to));

    void pokeDone(bool done) { setDone(done); }

    void pokeStatus(const Status& status) { setStatus(status); }
};

class DataReaderTest : public testing::Test {
  public:
    void readIntoBufferHelper(const std::string& data, int64 max_bytes, char* storage) {
        memcpy(storage, data.c_str(), data.size());
    }
};

TEST_F(DataReaderTest, TestCallback) {
    auto called = false;
    {
        // |reader| will be deleted_ leaving the current scope, therefore the
        // destructor gets called. Inside the destructor we expect that the
        // given callback got executed
        MockDataReader reader(kwc::MakeCallback(&Raise, &called));
    }
    EXPECT_TRUE(called);
}

TEST_F(DataReaderTest, TestAttributes) {
    MockDataReader reader;
    EXPECT_TRUE(reader.ok());
    EXPECT_TRUE(reader.status().ok());
    EXPECT_FALSE(reader.isDone());
    EXPECT_FALSE(reader.error());
    EXPECT_EQ(0, reader.getOffset());
    EXPECT_EQ(-1, reader.getTotalLength());

    reader.pokeDone(true);
    EXPECT_TRUE(reader.ok());
    EXPECT_TRUE(reader.status().ok());
    EXPECT_TRUE(reader.isDone());
    EXPECT_FALSE(reader.error());

    reader.pokeDone(false);
    EXPECT_TRUE(reader.ok());
    EXPECT_TRUE(reader.status().ok());
    EXPECT_FALSE(reader.isDone());
    EXPECT_FALSE(reader.error());

    auto status = Status(kwc::base::error::UNKNOWN, "Poked error");
    reader.pokeStatus(status);
    EXPECT_TRUE(reader.isDone());
    EXPECT_TRUE(reader.error());
    EXPECT_FALSE(reader.ok());
    EXPECT_EQ(status.toString(), reader.status().toString());

    reader.pokeStatus(Status());
    EXPECT_TRUE(reader.isDone());
    EXPECT_FALSE(reader.error());
    EXPECT_TRUE(reader.ok());
    EXPECT_TRUE(reader.status().ok());
}

TEST_F(DataReaderTest, CheckOffset) {
    MockDataReader reader;
    for (auto off : {5, 10, 20}) {
        EXPECT_CALL(reader, doSetOffset(off)).WillOnce((Return(off)));
        EXPECT_EQ(off, reader.setOffset(off));
        EXPECT_EQ(off, reader.getOffset());
    }

    EXPECT_CALL(reader, doSetOffset(10)).WillOnce(Return(-1));
    EXPECT_EQ(-1, reader.setOffset(10));
    EXPECT_EQ(-1, reader.getOffset());
    EXPECT_FALSE(reader.ok());
}

TEST_F(DataReaderTest, ReadEmptyToString) {
    MockDataReader reader;
    const std::string prefix(" ");
    std::string s = prefix;
    const int64 internal_buffer_size = 1 << 13;

    std::unique_ptr<Callback> poke_done(
        kwc::MakePermanentCallback(&reader, &MockDataReader::pokeDone, true));
    EXPECT_CALL(reader, doReadIntoBuffer(internal_buffer_size, _))
        .WillOnce(DoAll(InvokeWithoutArgs(poke_done.get(), &Callback::run), Return(0)));
    EXPECT_EQ(0, reader.readIntoString(kINT64max, &s));
    EXPECT_EQ(0, reader.getOffset());
    EXPECT_TRUE(reader.ok());
    EXPECT_TRUE(reader.status().ok());
    EXPECT_FALSE(reader.error());
    EXPECT_TRUE((reader.isDone()));
    EXPECT_EQ(prefix, s);
}

TEST_F(DataReaderTest, ReadIntoBuffer) {
    MockDataReader reader;
    const std::string expect("Lorem ipsum dolor");

    std::unique_ptr<Callback> poke_done(
        kwc::MakePermanentCallback(&reader, &MockDataReader::pokeDone, true));
    std::unique_ptr<ReadCallback> read_helper(
        kwc::MakePermanentCallback(this, &DataReaderTest::readIntoBufferHelper, expect));
    EXPECT_CALL(reader, doReadIntoBuffer(100, _))
        .WillOnce(DoAll(Invoke(read_helper.get(), &ReadCallback::run),
                        InvokeWithoutArgs(poke_done.get(), &Callback::run),
                        Return(static_cast<int64>(expect.size()))));
    char buffer[100];
    EXPECT_EQ(expect.size(), reader.readIntoBuffer(sizeof(buffer), buffer));
    EXPECT_EQ(expect.size(), reader.getOffset());
    EXPECT_FALSE(reader.error());
    EXPECT_TRUE(reader.ok());
    EXPECT_TRUE(reader.isDone());
    EXPECT_EQ(expect, std::string(buffer, expect.size()));
}

TEST_F(DataReaderTest, ReadIntoFragmentedString) {
    MockDataReader reader;
    const std::string prefix = " ";
    std::string s = prefix;

    const std::string lorem("Lorem ");
    const std::string ipsum("ipsum ");

    std::unique_ptr<Callback> poke_done(
        kwc::MakePermanentCallback(&reader, &MockDataReader::pokeDone, true));
    std::unique_ptr<ReadCallback> lorem_helper(
        kwc::MakePermanentCallback(this, &DataReaderTest::readIntoBufferHelper, lorem));
    std::unique_ptr<ReadCallback> ipsum_helper(
        kwc::MakePermanentCallback(this, &DataReaderTest::readIntoBufferHelper, ipsum));
    EXPECT_CALL(reader, doReadIntoBuffer(20, _))
        .WillOnce(DoAll(Invoke(lorem_helper.get(), &ReadCallback::run),
                        Return(static_cast<int64>(lorem.size()))));

    EXPECT_CALL(reader, doReadIntoBuffer(20 - lorem.size(), _))
        .WillOnce(DoAll(Invoke(ipsum_helper.get(), &ReadCallback::run),
                        InvokeWithoutArgs(poke_done.get(), &Callback::run),
                        Return(static_cast<int64>(ipsum.size()))));

    EXPECT_EQ(lorem.size() + ipsum.size(), reader.readIntoString(20, &s));
    EXPECT_EQ(prefix + lorem + ipsum, s);
    EXPECT_EQ(lorem.size() + ipsum.size(), reader.getOffset());
    EXPECT_TRUE(reader.ok());
    EXPECT_TRUE(reader.isDone());
    EXPECT_FALSE(reader.error());
}

TEST_F(DataReaderTest, ReadIntoFragmentedBuffer) {
    MockDataReader reader;
    char buffer[100];

    const std::string hello("Hello, ");
    const std::string world("world!");

    std::unique_ptr<Callback> poke_done(
        kwc::MakePermanentCallback(&reader, &MockDataReader::pokeDone, true));
    std::unique_ptr<ReadCallback> hello_helper(
        kwc::MakePermanentCallback(this, &DataReaderTest::readIntoBufferHelper, hello));
    std::unique_ptr<ReadCallback> world_helper(
        kwc::MakePermanentCallback(this, &DataReaderTest::readIntoBufferHelper, world));

    EXPECT_CALL(reader, doReadIntoBuffer(sizeof(buffer), _))
        .WillOnce(DoAll(Invoke(hello_helper.get(), &ReadCallback::run),
                        Return(static_cast<int64>(hello.size()))));

    EXPECT_CALL(reader, doReadIntoBuffer(sizeof(buffer) - hello.size(), _))
        .WillOnce(DoAll(Invoke(world_helper.get(), &ReadCallback::run),
                        InvokeWithoutArgs(poke_done.get(), &Callback::run),
                        Return(static_cast<int64>(world.size()))));

    EXPECT_EQ(hello.size() + world.size(), reader.readIntoBuffer(sizeof(buffer), buffer));
    EXPECT_EQ(hello + world, std::string(buffer, hello.size() + world.size()));
    EXPECT_EQ(hello.size() + world.size(), reader.getOffset());
    EXPECT_TRUE(reader.isDone());
    EXPECT_TRUE(reader.ok());
    EXPECT_FALSE(reader.error());
}

TEST_F(DataReaderTest, TestInvalidDataReader) {
    auto status = Status(kwc::base::error::UNKNOWN, "test");
    std::unique_ptr<DataReader> reader(
        kwc::serialization::CreateUnmanagedInvalidDataReader(status));
    EXPECT_FALSE(reader->ok());
    EXPECT_TRUE(reader->isDone());
    EXPECT_GT(0, reader->getTotalLength());
}