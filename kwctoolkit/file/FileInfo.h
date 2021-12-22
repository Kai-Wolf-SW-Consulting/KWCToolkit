// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_FILE_FILE_INFO_H_
#define KWCTOOLKIT_FILE_FILE_INFO_H_

#include <sys/stat.h>

#include <kwctoolkit/file/FilePath.h>

namespace kwc {
namespace file {

class FileInfo {
  public:
    FileInfo();
    bool isDirectory() const;
    FilePath getName() const;
    void setName(const FilePath& name);
    void clearStat();
    struct stat& stat();

  private:
    struct stat stat_;
    FilePath file_name_;
};

}  // namespace file
}  // namespace kwc

#endif  // KWCTOOLKIT_FILE_FILE_INFO_H_
