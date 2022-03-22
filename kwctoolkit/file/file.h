// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_FILE_FILE_H_
#define KWCTOOLKIT_FILE_FILE_H_

#include <cstdio>
#include <string>

namespace kwc {
namespace file {
class FilePath;

// Wrapper around OS-level file
//
// File access is implemented using fopen(), fgets(), fclose() etc, as opposed to be using
// open() or close() for several reasons:
// fopen() etc. provide buffering I/O which usually turns out to be a lot faster. Also
// these functions do line ending translation, if the file has not be opened in binary mode.
// A FILE* enables the usage of fscanf(), fgets() and other stdio functions.
//
// This class provides rather *low-level* operations (like read/write some bytes etc.).
// For more advanced stuff such as reading a whole file into a std::string, read file line
// by line better use the functionality provided in FileUtils.h instead.
class File {
  public:
    // Opens file with flags "r", "r+", "w", "w+". "a", and "a+"
    static File* open(const FilePath& name, const char* const flag);
    bool isOpen() const;
    size_t read(void* const buffer, size_t size);
    size_t write(const void* const buffer, size_t size);
    bool close();
    bool flush();
    size_t getSize() const;
    std::string getAbsoluteFileName() const;
    static bool remove(const FilePath& name);
    static bool exists(const FilePath& name);

  private:
    File(FILE* const descriptor, std::string name);
    FILE* file_;
    const std::string name_;
};

}  // namespace file
}  // namespace kwc

#endif  // KWCTOOLKIT_FILE_FILE_H_
