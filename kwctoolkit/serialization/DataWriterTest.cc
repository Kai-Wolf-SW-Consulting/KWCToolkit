// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/serialization/DataWriter.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <type_traits>

#include "kwctoolkit/base/Callback.h"
#include "kwctoolkit/base/IntegralTypes.h"
#include "kwctoolkit/base/Status.h"
#include "kwctoolkit/serialization/DataReader.h"

using namespace kwc;
using kwc::Callback;
using kwc::base::Status;
using kwc::serialization::DataReader;
using kwc::serialization::DataWriter;

using testing::_;
using testing::Return;

class MockDataWriter : public DataWriter {
  public:
    MockDataWriter() = default;
    ~MockDataWriter() override = default;

    MOCK_METHOD0(doClear, Status());
    MOCK_METHOD0(doBegin, Status());
    MOCK_METHOD0(doEnd, Status());
    MOCK_METHOD2(doWrite, Status(int64 bytes, const char* buffer));
    MOCK_METHOD1(doCreateDataReader, DataReader*(Callback* delete_cb));
};

TEST(DataWriterTest, TestMethods) {
    MockDataWriter writer;

    EXPECT_EQ(0, writer.getSize());
    EXPECT_CALL(writer, doBegin()).WillOnce(Return(Status()));
    writer.begin();

    const char write_data[] = "test_write";
    const int64 write_len = 4;

    EXPECT_CALL(writer, doWrite(write_len, write_data)).WillOnce(Return(Status()));
    EXPECT_TRUE(writer.writeData(write_len, write_data).ok());
    EXPECT_EQ(write_len, writer.getSize());

    EXPECT_CALL(writer, doWrite(write_len, write_data)).WillOnce(Return(Status()));
    EXPECT_TRUE(writer.writeData(write_len, write_data).ok());
    EXPECT_EQ(2 * write_len, writer.getSize());

    EXPECT_CALL(writer, doEnd()).WillOnce(Return(Status()));
    writer.end();

    EXPECT_EQ(2 * write_len, writer.getSize());
    EXPECT_CALL(writer, doClear()).WillOnce(Return(Status()));
    writer.clear();
    EXPECT_EQ(0, writer.getSize());
}

TEST(DataWriterTest, TestBeginAuto) {
    MockDataWriter writer;
    const char write_data[] = "test_write";
    const int64 write_len = 4;

    EXPECT_CALL(writer, doBegin()).WillOnce(Return(Status()));
    EXPECT_CALL(writer, doWrite(write_len, write_data)).WillOnce(Return(Status()));

    EXPECT_TRUE(writer.writeData(write_len, write_data).ok());
    EXPECT_EQ(write_len, writer.getSize());

    EXPECT_CALL(writer, doWrite(write_len, write_data)).WillOnce(Return(Status()));
    EXPECT_TRUE(writer.writeData(write_len, write_data).ok());
    EXPECT_EQ(2 * write_len, writer.getSize());
}

TEST(DataWriterTest, TestWriteReaderMemory) {
    const char* expect = "Lorem ipsum";
    auto* str = new std::string(expect);
    Callback* delete_cb = kwc::DeletePointerCallback(str);
    std::unique_ptr<DataWriter> writer(kwc::serialization::CreateStringDataWriter(str));
    std::unique_ptr<DataReader> reader(writer->createManagedDataReader(delete_cb));
    delete writer.release();
    EXPECT_EQ(std::string(expect), reader->readRemainingToString());
}
