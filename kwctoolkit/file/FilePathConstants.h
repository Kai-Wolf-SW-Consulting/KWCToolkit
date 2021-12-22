// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_FILE_FILE_PATH_CONSTANTS_H_
#define KWCTOOLKIT_FILE_FILE_PATH_CONSTANTS_H_

namespace kwc {
namespace file {

constexpr auto STRING_TERMINATOR = '\0';
constexpr auto PATH_SEPARATOR = "/";
constexpr auto PATH_SEP_LENGTH = 2;
constexpr auto EXTENSION_SEPARATOR = ".";
constexpr auto CURRENT_DIRECTORY = ".";
constexpr auto PARENT_DIRECTORY = "..";

}  // namespace file
}  // namespace kwc

#endif  // KWCTOOLKIT_FILE_FILE_PATH_CONSTANTS_H_
