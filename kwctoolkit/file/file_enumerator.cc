// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/file/file_enumerator.h"

#include <dirent.h>
#include <fnmatch.h>

#include <cerrno>

#include "kwctoolkit/file/file_path_constants.h"

namespace kwc {
namespace file {

FileEnumerator::FileEnumerator(const FilePath& root_path, bool recursive, int file_type)
    : current_dir_entry_(0), root_path_(root_path), recursive_(recursive), type_(file_type) {
    pending_paths_.push(root_path);
}

FileEnumerator::FileEnumerator(const FilePath& root_path,
                               bool recursive,
                               int file_type,
                               const FilePath::StringType& pattern)
    : current_dir_entry_(0),
      root_path_(root_path),
      recursive_(recursive),
      type_(file_type),
      pattern_(root_path.append(pattern).value()) {
    if (pattern.empty()) {
        pattern_ = FilePath::StringType();
    }
    pending_paths_.push(root_path);
}

FilePath FileEnumerator::next() {
    ++current_dir_entry_;

    while (current_dir_entry_ >= dir_entries_.size()) {
        if (pending_paths_.empty()) {
            return {};
        }

        root_path_ = pending_paths_.top();
        root_path_ = root_path_.stripTrailingSeparators();
        pending_paths_.pop();

        std::vector<FileInfo> entries;
        if (!readDirectory(&entries, root_path_, type_ & SHOW_SYM_LINKS)) {
            continue;
        }

        dir_entries_.clear();
        current_dir_entry_ = 0;
        for (auto iter = entries.begin(); iter != entries.end(); ++iter) {
            auto full_path = root_path_.append(iter->getName());
            if (skip(full_path)) {
                continue;
            }

            if (pattern_.size() &&
                fnmatch(pattern_.c_str(), full_path.value().c_str(), FNM_NOESCAPE)) {
                continue;
            }

            if (recursive_ && S_ISDIR(iter->stat().st_mode)) {
                pending_paths_.push(full_path);
            }

            if ((S_ISDIR(iter->stat().st_mode) && (type_ & DIRECTORIES)) ||
                (!S_ISDIR(iter->stat().st_mode) && (type_ & FILES))) {
                dir_entries_.push_back(*iter);
            }
        }
    }
    return root_path_.append(dir_entries_[current_dir_entry_].getName());
}

bool FileEnumerator::skip(const FilePath& path) {
    auto basename = path.baseName().value();
    return basename == kCurrentDirectory ||
           (basename == kParentDirectory && !(INCLUDE_DOT_DOT & type_));
}

bool FileEnumerator::readDirectory(std::vector<FileInfo>* entries,
                                   const FilePath& source,
                                   bool show_links) {
    DIR* dir = opendir(source.value().c_str());
    if (!dir) {
        return false;
    }

    struct dirent dent_buf;
    struct dirent* dent;
    while (readdir_r(dir, &dent_buf, &dent) == 0 && dent) {
        FileInfo info;
        info.setName(FilePath(dent->d_name));

        FilePath full_name = source.append(dent->d_name);
        int ret;
        if (show_links) {
            ret = lstat(full_name.value().c_str(), &info.stat());
        } else {
            ret = stat(full_name.value().c_str(), &info.stat());
        }

        if (ret < 0) {
            // TODO(kwc): log errno = ENOENT error message
            info.clearStat();
        }
        entries->push_back(info);
    }
    closedir(dir);
    return true;
}

}  // namespace file
}  // namespace kwc