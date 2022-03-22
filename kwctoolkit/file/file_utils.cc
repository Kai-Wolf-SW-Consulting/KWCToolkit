// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/file/file_utils.h"

#include <vector>

#if defined(KWC_OS_WINDOWS)
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

FilePath GetTempDir() {
    const char* tmp = getenv("TMPDIR");
    if (tmp) {
        return FilePath(tmp);
    }

    return FilePath("/tmp");
}

bool DirectoryExists(const FilePath& path) {
    struct stat64 file_info;
    if (stat64(path.value().c_str(), &file_info) == 0) {
        return S_ISDIR(file_info.st_mode);
    }
    return false;
}

bool CreateDirectory(const FilePath& full_path) {
    std::vector<FilePath> sub_paths;
    // collect list of all parent directories
    FilePath last_path = full_path;
    sub_paths.push_back(full_path);
    for (FilePath path = full_path.directoryName(); path.value() != last_path.value();
         path = path.directoryName()) {
        sub_paths.push_back(path);
        last_path = path;
    }

    // iterate through parents and create missing directories
    for (auto iter = sub_paths.rbegin(); iter != sub_paths.rend(); ++iter) {
        if (DirectoryExists(*iter)) {
            continue;
        }

        if (mkdir(iter->value().c_str(), 0700) == 0) {
            continue;
        }

        // mkdir failed, but it might be because of EEXIST (does
        // already exist). Therefore we check to see, if it exists and
        // make sure it is a directory
        if (!DirectoryExists(*iter)) {
            return false;
        }
    }
    return true;
}

#if defined(KWC_OS_MACOS) || defined(KWC_OS_LINUX)
int OpenNoInterrupt(const char* name, int flags, mode_t mode) {
    return int(internal::WrapNoInterrupt(open, name, flags, mode));
}

int CloseNoInterrupt(int fd) {
    return IgnoreCloseReturn(close(fd));
}

ssize_t ReadFull(int fd, void* buf, std::size_t count) {
    return internal::WrapIncomplete(read, fd, buf, count);
}

ssize_t WriteFull(int fd, const void* buf, std::size_t count) {
    return internal::WrapIncomplete(write, fd, const_cast<void*>(buf), count);
}
#endif

}  // namespace file
}  // namespace kwc