// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

#include <kwctoolkit/file/File.h>
#include <kwctoolkit/file/FilePathConstants.h>
#include <kwctoolkit/file/FileUtils.h>
#include <kwctoolkit/strings/StringUtils.h>

using namespace kwc;
using namespace kwc::file;

class FileTest : public testing::Test {
  protected:
    virtual void SetUp() {
        tmp_file = GetTempDir().append(file_name);
        file = std::unique_ptr<File>(File::open(tmp_file, "w"));
    }

    std::string file_name{"kwc_file_test.txt"};
    FilePath tmp_file;
    std::unique_ptr<File> file;
};

TEST_F(FileTest, CreateAndClose) {
    ASSERT_TRUE(file != nullptr);
    EXPECT_TRUE(file->isOpen());

    ASSERT_TRUE(file->close());
    EXPECT_FALSE(file->isOpen());
}

TEST_F(FileTest, RemoveOpenFile) {
    EXPECT_TRUE(file->isOpen());
    EXPECT_TRUE(File::exists(tmp_file));
    EXPECT_TRUE(File::remove(tmp_file));
    EXPECT_FALSE(File::exists(tmp_file));
}

TEST_F(FileTest, GetFileName) {
    ASSERT_TRUE(strings::EndsWith(file->getAbsoluteFileName(), file_name));
}

TEST_F(FileTest, GetFileSize) {
    EXPECT_EQ(0, file->getSize());
    const std::string foo("This is a foo");
    EXPECT_EQ(foo.size(), file->write(foo.c_str(), foo.size()));
    EXPECT_GE(0, file->getSize());
}

TEST_F(FileTest, WriteAndReadFile) {
    const std::string foo("This is another foo");
    auto size = foo.size();
    EXPECT_EQ(size, file->write(foo.c_str(), foo.size()));
    EXPECT_TRUE(file->close());

    std::unique_ptr<File> file2(File::open(tmp_file, "r"));
    std::vector<char> buff(size);
    EXPECT_TRUE(file2->isOpen());
    auto chars_read = file2->read(&buff[0], buff.size());
    EXPECT_EQ(size, chars_read);
    std::string res(buff.begin(), buff.begin() + chars_read);
    EXPECT_EQ(foo, res);
}
