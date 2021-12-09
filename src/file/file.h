// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_FILE_FILE_H_
#define KWCTOOLKIT_FILE_FILE_H_

#include <sys/fcntl.h>
#include <sys/types.h>

namespace kwc {
namespace file {
class FilePath;

class File {
  public:
    File() noexcept;
    explicit File(int fd, bool owns_fd = false) noexcept;
    explicit File(const FilePath& filename, int flags = O_RDONLY, mode_t mode = 0666);

    int fd() const;
    explicit operator bool() const;

    void close();
    int release() noexcept;

    File(File&&) noexcept;
    File& operator=(File&&);
    void swap(File& other) noexcept;

  private:
    File(const File&) = delete;
    File& operator=(const File&) = delete;

    int fd_;
    bool owns_fd_;
};

}  // namespace file
}  // namespace kwc

#endif  // KWCTOOLKIT_FILE_FILE_H_
