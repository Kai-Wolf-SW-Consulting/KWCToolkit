// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/file/file.h"

#include <gtest/gtest.h>

#include <memory>
#include <string>
#include <vector>

#include "kwctoolkit/file/file_path_constants.h"
#include "kwctoolkit/file/file_utils.h"
#include "kwctoolkit/strings/string_utils.h"

using namespace kwc;
using namespace kwc::file;

class FileTest : public testing::Test {
  protected:
    void SetUp() override {
        tmp_file_ = GetTempDir().append(file_name_);
        file_ = std::unique_ptr<File>(File::open(tmp_file_, "w"));
    }

    std::string file_name_{"kwc_file_test.txt"};
    FilePath tmp_file_;
    std::unique_ptr<File> file_;
};

TEST_F(FileTest, CreateAndClose) {
    ASSERT_TRUE(file_ != nullptr);
    EXPECT_TRUE(file_->isOpen());

    ASSERT_TRUE(file_->close());
    EXPECT_FALSE(file_->isOpen());
}

TEST_F(FileTest, RemoveOpenFile) {
    EXPECT_TRUE(file_->isOpen());
    EXPECT_TRUE(File::exists(tmp_file_));
    EXPECT_TRUE(File::remove(tmp_file_));
    EXPECT_FALSE(File::exists(tmp_file_));
}

TEST_F(FileTest, GetFileName) {
    ASSERT_TRUE(strings::EndsWith(file_->getAbsoluteFileName(), file_name_));
}

TEST_F(FileTest, GetFileSize) {
    EXPECT_EQ(0, file_->getSize());
    const std::string foo("This is a foo_");
    EXPECT_EQ(foo.size(), file_->write(foo.c_str(), foo.size()));
    EXPECT_GE(0, file_->getSize());
}

TEST_F(FileTest, WriteAndReadFile) {
    const std::string foo("This is another foo_");
    auto size = foo.size();
    EXPECT_EQ(size, file_->write(foo.c_str(), foo.size()));
    EXPECT_TRUE(file_->close());

    std::unique_ptr<File> file2(File::open(tmp_file_, "r"));
    std::vector<char> buff(size);
    EXPECT_TRUE(file2->isOpen());
    auto chars_read = file2->read(&buff[0], buff.size());
    EXPECT_EQ(size, chars_read);
    std::string res(buff.begin(), buff.begin() + chars_read);
    EXPECT_EQ(foo, res);
}
