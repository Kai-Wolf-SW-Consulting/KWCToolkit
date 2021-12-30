// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/file/FileEnumerator.h"

#include <gtest/gtest.h>

#include <fstream>
#include <set>

#include "kwctoolkit/file/File.h"
#include "kwctoolkit/file/FileUtils.h"

using namespace kwc::file;

// Collects results from given file enumerator nad provides a query interface
// to look if a given file is present
class FindResultCollector {
  public:
    explicit FindResultCollector(FileEnumerator* enumerator) {
        FilePath current_file;
        while (!(current_file = enumerator->next()).value().empty()) {
            const auto& path = current_file.value();
            // the file should not be returned twice
            EXPECT_TRUE(files_.end() == files_.find(path)) << "Same file returned twice";
            files_.insert(path);
        }
    }

    // returns true, if the enumerator has found the file
    bool hasFile(const FilePath& file_path) const {
        return files_.find(file_path.value()) != files_.end();
    }

    int size() { return static_cast<int>(files_.size()); }

  private:
    std::set<FilePath::StringType> files_;
};

class FileEnumeratorTest : public testing::Test {
  protected:
    void SetUp() override {
        tmp_dir_ = GetTempDir().append("fe-test");
        ASSERT_TRUE(CreateDirectory(tmp_dir_));
    }

    void createFile(const FilePath& path) {
        std::ofstream file;
        file.open(path.value().c_str());
        ASSERT_TRUE(file.is_open());
        file.close();
    }

    FilePath tmp_dir_;
};

TEST_F(FileEnumeratorTest, EmptyDirectory) {
    FileEnumerator fe(tmp_dir_, true, FileType::FILES | FileType::DIRECTORIES);
    EXPECT_EQ("", fe.next().value());
    EXPECT_EQ("", fe.next().value());
}

TEST_F(FileEnumeratorTest, EmptyDirectoryWithDotDot) {
    FileEnumerator fe(tmp_dir_, false,
                      FileType::FILES | FileType::DIRECTORIES | FileType::INCLUDE_DOT_DOT);
    EXPECT_EQ(tmp_dir_.append("..").value(), fe.next().value());
    EXPECT_EQ("", fe.next().value());
}

TEST_F(FileEnumeratorTest, EnumerateDirectories) {
    auto dir1 = tmp_dir_.append("dir1");
    auto dir2 = tmp_dir_.append("dir2");
    auto dir2_inner = tmp_dir_.append("inner");
    EXPECT_TRUE(CreateDirectory(dir1));
    EXPECT_TRUE(CreateDirectory(dir2));
    EXPECT_TRUE(CreateDirectory(dir2_inner));

    FileEnumerator fe(tmp_dir_, true, FileType::DIRECTORIES);
    FindResultCollector cl(&fe);
    EXPECT_TRUE(cl.hasFile(dir1));
    EXPECT_TRUE(cl.hasFile(dir2));
    EXPECT_TRUE(cl.hasFile(dir2_inner));
    EXPECT_EQ(3, cl.size());

    // clean up after test is finished
    EXPECT_TRUE(File::remove(dir1));
    EXPECT_TRUE(File::remove(dir2));
    EXPECT_TRUE(File::remove(dir2_inner));
}

TEST_F(FileEnumeratorTest, EnumerateDirectoriesNonRecursive) {
    auto dir1 = tmp_dir_.append("dir1");
    auto dir2 = tmp_dir_.append("dir2");
    EXPECT_TRUE(CreateDirectory(dir1));
    EXPECT_TRUE(CreateDirectory(dir2));

    FileEnumerator fe(tmp_dir_, false, FileType::DIRECTORIES);
    FindResultCollector cl(&fe);
    EXPECT_TRUE(cl.hasFile(dir1));
    EXPECT_TRUE(cl.hasFile(dir2));
    EXPECT_EQ(2, cl.size());

    // clean up after test is finished
    EXPECT_TRUE(File::remove(dir1));
    EXPECT_TRUE(File::remove(dir2));
}

TEST_F(FileEnumeratorTest, EnumerateDirectoriesNonRecursiveWithDotDot) {
    auto dir1 = tmp_dir_.append("dir1");
    auto dir2 = tmp_dir_.append("dir2");
    EXPECT_TRUE(CreateDirectory(dir1));
    EXPECT_TRUE(CreateDirectory(dir2));

    FileEnumerator fe(tmp_dir_, false, FileType::DIRECTORIES | FileType::INCLUDE_DOT_DOT);
    FindResultCollector cl(&fe);
    EXPECT_TRUE(cl.hasFile(dir1));
    EXPECT_TRUE(cl.hasFile(dir2));
    EXPECT_TRUE(cl.hasFile(tmp_dir_.append("..")));
    EXPECT_EQ(3, cl.size());

    // clean up after test is finished
    EXPECT_TRUE(File::remove(dir1));
    EXPECT_TRUE(File::remove(dir2));
}

TEST_F(FileEnumeratorTest, EnumerateDirectoriesWithPattern) {
    auto dir1 = tmp_dir_.append("dir1");
    auto dir2 = tmp_dir_.append("dir2");
    auto dir3 = tmp_dir_.append("foo1");
    EXPECT_TRUE(CreateDirectory(dir1));
    EXPECT_TRUE(CreateDirectory(dir2));
    EXPECT_TRUE(CreateDirectory(dir3));

    FileEnumerator fe(tmp_dir_, true, FileType::FILES | FileType::DIRECTORIES, "dir*");
    FindResultCollector cl(&fe);
    EXPECT_TRUE(cl.hasFile(dir1));
    EXPECT_TRUE(cl.hasFile(dir2));
    EXPECT_FALSE(cl.hasFile(dir3));
    EXPECT_EQ(2, cl.size());

    // clean up after test is finished
    EXPECT_TRUE(File::remove(dir1));
    EXPECT_TRUE(File::remove(dir2));
    EXPECT_TRUE(File::remove(dir3));
}

TEST_F(FileEnumeratorTest, EnumerateFiles) {
    auto dir1 = tmp_dir_.append("dir1");
    EXPECT_TRUE(CreateDirectory(dir1));
    auto dir1file = tmp_dir_.append("dir1").append("file1.csv");
    createFile(dir1file);

    auto dir2 = tmp_dir_.append("dir2");
    EXPECT_TRUE(CreateDirectory(dir2));
    auto dir2file = tmp_dir_.append("dir2").append("dir2file.txt");
    createFile(dir2file);

    auto dir2_inner = dir2.append("inner");
    EXPECT_TRUE(CreateDirectory(dir2_inner));
    auto dir2inner_file = dir2_inner.append("innerfile.ini");
    createFile(dir2inner_file);

    FileEnumerator fe(tmp_dir_, true, FileType::FILES);
    FindResultCollector cl(&fe);
    EXPECT_TRUE(cl.hasFile(dir1file));
    EXPECT_TRUE(cl.hasFile(dir2file));
    EXPECT_TRUE(cl.hasFile(dir2inner_file));
    EXPECT_EQ(3, cl.size());

    // clean up after test is finished
    EXPECT_TRUE(File::remove(dir1file));
    EXPECT_TRUE(File::remove(dir1));
    EXPECT_TRUE(File::remove(dir2inner_file));
    EXPECT_TRUE(File::remove(dir2file));
    EXPECT_TRUE(File::remove(dir2_inner));
    EXPECT_TRUE(File::remove(dir2));
}
