// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "file/file_utils.h"

#include "base/platform.h"

#if defined(OS_WINDOWS)
    #include <direct.h>
    #include <io.h>
    #define close _close
#else
    #include <unistd.h>
#endif

namespace kwc {
namespace file {
namespace {
int IgnoreCloseReturn(int r) {
    // Ignore EINTR. According to POSIX, close() function may only return EINTR after the file
    // descriptor has been closed, so we must not retry close() in EINTR.
    if (r == -1 && errno == EINTR) {
        return 0;
    }
    return r;
}
}  // namespace

int openNoInterrupt(const char* name, int flags, mode_t mode) {
    return int(internal::wrapNoInterrupt(open, name, flags, mode));
}

int closeNoInterrupt(int fd) {
    return IgnoreCloseReturn(close(fd));
}

ssize_t readFull(int fd, void* buf, std::size_t count) {
    return internal::wrapIncomplete(read, fd, buf, count);
}

ssize_t writeFull(int fd, const void* buf, std::size_t count) {
    return internal::wrapIncomplete(write, fd, const_cast<void*>(buf), count);
}

}  // namespace file
}  // namespace kwc