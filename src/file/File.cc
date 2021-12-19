// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "file/File.h"

#include <sys/stat.h>
#include <sys/types.h>

#include <cstdio>

#include "file/FilePath.h"

#if defined(OS_WINDOWS)
    #include <direct.h>
    #include <io.h>
    #define close _close
#else
    #include <unistd.h>
#endif

namespace kwc {
namespace file {

File::File(FILE* const descriptor, const std::string& name) : file_(descriptor), name_(name) {}

File* File::open(const FilePath& name, const char* const flag) {
    FILE* const fd = fopen(name.value().c_str(), flag);
    if (fd == nullptr)
        return nullptr;
    File* const f = new File(fd, name.value());
    return f;
}

bool File::isOpen() const {
    return file_ != nullptr;
}

std::size_t File::read(void* const buffer, std::size_t size) {
    return fread(buffer, 1, size, file_);
}

std::size_t File::write(const void* const buffer, std::size_t size) {
    return fwrite(buffer, 1, size, file_);
}

bool File::close() {
    if (fclose(file_) == 0) {
        file_ = nullptr;
        return true;
    }
    return false;
}

bool File::flush() {
    return fflush(file_) == 0;
}

std::size_t File::getSize() const {
    struct stat f_stat;
    stat(name_.c_str(), &f_stat);
    return static_cast<std::size_t>(f_stat.st_size);
}

std::string File::getAbsoluteFileName() const {
    return name_;
}

bool File::remove(FilePath name) {
    return ::remove(name.value().c_str()) == 0;
}

bool File::exists(const FilePath& name) {
    return access(name.value().c_str(), F_OK) == 0;
}

}  // namespace file
}  // namespace kwc