// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_FILE_FILE_UTILS_H_
#define KWCTOOLKIT_FILE_FILE_UTILS_H_

#include <algorithm>
#include <cstdio>
#include <limits>
#include <memory>

#include "kwctoolkit/base/assert.h"
#include "kwctoolkit/base/check.h"
#include "kwctoolkit/base/compiler.h"
#include "kwctoolkit/base/platform.h"
#include "kwctoolkit/base/scope_guard.h"
#include "kwctoolkit/file/file_path.h"

#if defined(KWC_OS_POSIX) || defined(KWC_OS_LINUX)
    #include <sys/errno.h>
    #include <sys/fcntl.h>
    #include <sys/stat.h>
#endif

namespace kwc {
namespace file {

FilePath GetTempDir();

bool DirectoryExists(const FilePath& path);

bool CreateDirectory(const FilePath& full_path);

#if defined(KWC_OS_MACOS) || defined(KWC_OS_LINUX)
// These are wrappers around commonly used system calls so that they retry on EINTR.
// See WrapIncomplete below for read/write.
int OpenNoInterrupt(const char* name, int flags, mode_t mode = 0666);
int CloseNoInterrupt(int fd);

KWC_NO_DISCARD ssize_t ReadFull(int fd, void* buf, std::size_t count);
ssize_t WriteFull(int fd, const void* buf, std::size_t count);

namespace internal {
using ssize_t = int;

template <typename Func, typename... Args>
ssize_t WrapNoInterrupt(Func f, Args... args) {
    ssize_t r;
    do {
        r = f(args...);
    } while (r == -1 && errno == EINTR);
    return r;
}

// Wrap calls to read/write to retry on incomplete reads or writes.

template <typename Func>
ssize_t WrapIncomplete(Func f, int fd, void* buf, std::size_t count) {
    auto* b = static_cast<char*>(buf);
    ssize_t total_bytes = 0;
    ssize_t r;
    do {
        r = f(fd, b, count);
        if (r == -1) {
            if (errno == EINTR) {
                continue;
            }
            return r;
        }
        total_bytes += r;
        b += r;
        count -= r;
    } while (r != 0 && count);  // 0 means EOF

    return total_bytes;
}
}  // namespace internal

// Read either entire file or no more than num_bytes into container. The container is assumed to be
// contiguous with an element size equal to 1 byte and offers at most size(), reserve() as well as
// random access (e.g. std::vector<uint8>, std::string, etc).
template <typename Container>
bool ReadFile(int fd,
              Container& out,
              std::size_t num_bytes = std::numeric_limits<std::size_t>::max()) {
    KWC_ASSERT(sizeof(out[0] == 1, "ReadFile only accepts containers with byte-sized elements"));

    std::size_t read_so_far = 0;
    KWC_SCOPE_EXIT {
        KWC_CHECK(out.size() >= read_so_far);
        out.resize(read_so_far);
    };

    // Retrieve file size
    struct stat buf;
    if (fstat(fd, &buf) == -1) {
        return false;
    }

    // There are certain files (e.g. under /proc or /sys on Linux) that lie about their size.
    // Hence, we treat the size advertised by fstat under advise, but don't necessarily rely on it.
    // In particular, if the size is zero, we should attempt to read stuff regardless. If not zero,
    // we attempt to read one extra byte
    constexpr std::size_t initial_alloc = 1024 * 4;
    out.resize(
        std::min(buf.st_size > 0 ? (std::size_t(buf.st_size) + 1) : initial_alloc, num_bytes));

    while (read_so_far < out.size()) {
        const auto actual = ReadFull(fd, &out[read_so_far], out.size() - read_so_far);
        if (actual == -1) {
            return false;
        }
        read_so_far += actual;
        if (read_so_far < out.size()) {
            // File exhausted
            break;
        }

        // Ups, allocate more memory. Use exponential growth to avoid quadratic behavour. Cap size
        // to num_bytes
        out.resize(std::min(out.size() * 3 / 2, num_bytes));
    }

    return true;
}

// Read entire file or no more than num_bytes as above, but accepts a FilePath not descriptor
template <typename Container>
bool ReadFile(const FilePath& filename,
              Container& out,
              std::size_t num_bytes = std::numeric_limits<std::size_t>::max()) {
    const auto fd = OpenNoInterrupt(filename.value().c_str(), O_RDONLY | O_CLOEXEC);
    if (fd == -1) {
        return false;
    }

    KWC_SCOPE_EXIT { CloseNoInterrupt(fd); };

    return ReadFile(fd, out, num_bytes);
}

// Write container to file. As in ReadFile() the container is assumed to be contiguous with an
// element size equal to 1 byte and offers at most size(), reserve() as well as random access
template <typename Container>
bool WriteFile(const FilePath& filename,
               const Container& data,
               int flags = O_WRONLY | O_CREAT | O_TRUNC,
               mode_t mode = 0666) {
    KWC_ASSERT(sizeof(data[0] == 1, "WriteFile only accepts containers with byte-sized elements"));
    int fd = open(filename.value().c_str(), flags, mode);
    if (fd == -1) {
        return false;
    }

    bool ok =
        data.empty() || WriteFull(fd, &data[0], data.size()) == static_cast<ssize_t>(data.size());
    return CloseNoInterrupt(fd) == 0 && ok;
}

#endif

}  // namespace file
}  // namespace kwc

#endif  // KWCTOOLKIT_FILE_FILE_UTILS_H_
