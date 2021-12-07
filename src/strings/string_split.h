// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_STRINGS_STRING_SPLIT_H_
#define KWCTOOLKIT_STRINGS_STRING_SPLIT_H_

#include <string>
#include <utility>
#include <vector>

namespace kwc {
namespace strings {

enum WhitespaceHandling { WHITESPACE_KEEP, WHITESPACE_TRIM };

enum SplitResult { SPLIT_WANT_ALL, SPLIT_WANT_NONEMPTY };

std::vector<std::string> SplitString(const std::string& input,
                                     const std::string& separator,
                                     WhitespaceHandling whitespace = WHITESPACE_TRIM,
                                     SplitResult split_result = SPLIT_WANT_NONEMPTY);
}  // namespace strings
}  // namespace kwc

#endif  // KWCTOOLKIT_STRINGS_STRING_SPLIT_H_
