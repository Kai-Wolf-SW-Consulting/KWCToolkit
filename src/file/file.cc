// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "file/file.h"

#include <algorithm>

#include "base/check.h"
#include "base/logging.h"
#include "base/macros.h"

#if defined(OS_WINDOWS)
    #include <direct.h>
    #include <io.h>
    #define close _close
#else
    #include <unistd.h>
#endif

namespace kwc {
namespace file {

File::File() noexcept : fd_(-1), owns_fd_(false) {}

File::File(int fd, bool owns_fd) noexcept : fd_(fd), owns_fd_(owns_fd) {
    CHECK_GE(fd, -1) << "file descriptor must be -1 or non-negative";
    CHECK(fd != -1 || !owns_fd) << "cannot own -1";
}

File::File(const FilePath& filename, int flags, mode_t mode)
    : fd_(::open(filename.value().c_str(), flags, mode)), owns_fd_(false) {
    if (fd_ == -1) {
        LOGGING(base::ERROR) << "Error: open file failed on: " << filename.value() << "\n";
    } else {
        owns_fd_ = true;
    }
}

int File::fd() const {
    return fd_;
}

File::operator bool() const {
    return fd_ != -1;
}

void File::close() {
    owns_fd_ ? ::close(fd_) : 0;
    release();
}

int File::release() noexcept {
    int released = fd_;
    fd_ = -1;
    owns_fd_ = false;
    return released;
}

File::File(File&& other) noexcept : fd_(other.fd_), owns_fd_(other.owns_fd_) {
    other.release();
}

void File::swap(File& other) noexcept {
    using std::swap;
    swap(fd_, other.fd_);
    swap(owns_fd_, other.owns_fd_);
}

File& File::operator=(File&& other) {
    close();
    swap(other);
    return *this;
}

}  // namespace file
}  // namespace kwc