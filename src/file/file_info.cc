// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "file/file_info.h"

#include <cstring>

namespace kwc {
namespace file {

FileInfo::FileInfo() {
    clearStat();
}

bool FileInfo::isDirectory() const {
    return S_ISDIR(stat_.st_mode);
}

FilePath FileInfo::getName() const {
    return file_name_;
}

void FileInfo::setName(const FilePath& name) {
    file_name_ = name;
}

void FileInfo::clearStat() {
    memset(&stat_, 0, sizeof(stat_));
}

struct stat& FileInfo::stat() {
    return stat_;
}

}  // namespace file
}  // namespace kwc