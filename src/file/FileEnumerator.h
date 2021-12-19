// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_FILE_FILE_ENUMERATOR_H_
#define KWCTOOLKIT_FILE_FILE_ENUMERATOR_H_

#include <sys/stat.h>
#include <unistd.h>

#include <stack>
#include <vector>

#include "base/Macros.h"
#include "file/FileInfo.h"
#include "file/FilePath.h"

namespace kwc {
namespace file {

enum FileType {
    FILES = 1 << 0,
    DIRECTORIES = 1 << 1,
    INCLUDE_DOT_DOT = 1 << 2,
    SHOW_SYM_LINKS = 1 << 3,
};

class FileEnumerator {
  public:
    FileEnumerator(const FilePath& root_path, bool recursive, int file_type);
    FileEnumerator(const FilePath& root_path,
                   bool recursive,
                   int file_type,
                   const FilePath::StringType& pattern);
    FilePath next();

  private:
    bool skip(const FilePath& path);
    static bool readDirectory(std::vector<FileInfo>* entries,
                              const FilePath& source,
                              bool show_links);
    std::vector<FileInfo> dir_entries_;
    std::size_t current_dir_entry_;
    FilePath root_path_;
    bool recursive_;
    int type_;
    FilePath::StringType pattern_;
    std::stack<FilePath> pending_paths_;
    DISALLOW_COPY_AND_ASSIGN(FileEnumerator);
};
}  // namespace file
}  // namespace kwc

#endif  // KWCTOOLKIT_FILE_FILE_ENUMERATOR_H_
