// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_UTILS_BASE64_H_
#define KWCTOOLKIT_UTILS_BASE64_H_

#include <string>

namespace kwc {
namespace utils {

std::string Base64Encode(unsigned const char* bytes_to_encode, unsigned int in_len);
std::string Base64Decode(const std::string& encoded_string);

}  // namespace utils
}  // namespace kwc

#endif  // KWCTOOLKIT_UTILS_BASE64_H_
