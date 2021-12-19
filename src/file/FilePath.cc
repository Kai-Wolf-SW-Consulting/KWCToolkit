// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "file/FilePath.h"

#include <cstddef>

namespace kwc {
namespace file {

constexpr auto kStringTerminator = '\0';
constexpr FilePath::CharType kCurrentDirectory[] = ".";
#if defined(OS_WINDOWS)
constexpr FilePath::CharType kPathSeparator[] = "\\/";
constexpr auto kPathSepLength = 3;
#else
constexpr auto kPathSeparator = "/";
constexpr auto kPathSepLength = 2;
#endif

FilePath::FilePath(const FilePath& other) : path_(other.path_) {}

FilePath::FilePath(const FilePath::StringType& path) : path_(path) {
    const auto null_pos = path_.find(kStringTerminator);
    if (null_pos != std::string::npos) {
        path_.erase(null_pos, std::string::npos);
    }
}

FilePath& FilePath::operator=(const FilePath& other) = default;

bool FilePath::operator==(const FilePath& other) const {
    return path_ == other.path_;
}

bool FilePath::operator!=(const FilePath& other) const {
    return path_ != other.path_;
}

bool FilePath::operator<(const FilePath& other) const {
    return path_ < other.path_;
}

const FilePath::StringType& FilePath::value() const {
    return path_;
}

bool FilePath::isSeperator(FilePath::CharType ch) {
    for (std::size_t i = 0; i < kPathSepLength - 1; ++i) {
        if (ch == kPathSeparator[i]) {
            return true;
        }
    }
    return false;
}

FilePath FilePath::append(const std::string& component) const {
    auto appended = component;
    const auto null_pos = component.find(kStringTerminator);
    if (null_pos != std::string::npos) {
        appended = component.substr(0, null_pos);
    }

    if (path_.compare(kCurrentDirectory) == 0) {
        return FilePath(appended);
    }

    FilePath new_path(path_);
    new_path.stripTrailingSeparatorsInternal();

    // Don't append path separator if path or path component is empty
    if (appended.length() > 0 && new_path.path_.length() > 0) {
        // Don't append path separator for root directory
        if (!isSeperator(new_path.path_[new_path.path_.length() - 1])) {
            new_path.path_ += kPathSeparator;
        }
    }

    new_path.path_ += appended;
    return new_path;
}

FilePath FilePath::append(const FilePath& component) const {
    return append(component.value());
}

FilePath FilePath::baseName() const {
    FilePath new_path(path_);
    new_path.stripTrailingSeparatorsInternal();

    auto last_sep =
        new_path.path_.find_last_of(kPathSeparator, std::string::npos, kPathSepLength - 1);
    if (last_sep != std::string::npos && last_sep < new_path.path_.length() - 1) {
        new_path.path_.erase(0, last_sep + 1);
    }

    return new_path;
}

FilePath FilePath::directoryName() const {
    FilePath new_path(path_);
    new_path.stripTrailingSeparatorsInternal();

    const auto last_sep =
        new_path.path_.find_last_of(kPathSeparator, std::string::npos, kPathSepLength - 1);
    if (last_sep == std::string::npos) {
        // in current directory
        new_path.path_.resize(0);
    } else if (last_sep == 0) {
        // in root directory
        new_path.path_.resize(1);
    } else if (last_sep == 1 && isSeperator(new_path.path_[0])) {
        new_path.path_.resize(2);
    } else if (last_sep != 0) {
        // trim just basename
        new_path.path_.resize(last_sep);
    }

    new_path.stripTrailingSeparatorsInternal();
    if (!new_path.path_.length()) {
        new_path.path_ = kCurrentDirectory;
    }

    return new_path;
}

FilePath FilePath::stripTrailingSeparators() const {
    FilePath new_path(path_);
    new_path.stripTrailingSeparatorsInternal();

    return new_path;
}

void FilePath::stripTrailingSeparatorsInternal() {
    const auto start = 1;
    auto last_stripped = std::string::npos;
    for (auto pos = path_.length(); pos > start && isSeperator(path_[pos - 1]); --pos) {
        // don't strip if path starts with '//'
        if (pos != start + 1 || last_stripped == start + 2 || !isSeperator((path_[start - 1]))) {
            path_.resize(pos - 1);
            last_stripped = pos;
        }
    }
}

}  // namespace file
}  // namespace kwc