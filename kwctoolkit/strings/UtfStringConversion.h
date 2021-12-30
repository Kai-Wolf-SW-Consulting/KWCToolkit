// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_STRINGS_UTF_STRING_CONVERSION_H_
#define KWCTOOLKIT_STRINGS_UTF_STRING_CONVERSION_H_

#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <string>

// UTF8 conversion utility functions to translate between Windows/Unix

namespace kwc {
namespace strings {

bool WideToUTF8(const wchar_t* src, std::size_t src_len, std::string* output);

std::string WideToUTF8(const std::wstring& wide);

bool UTF8ToWide(const char* src, std::size_t src_len, std::wstring* output);

std::wstring UTF8ToWide(const std::string& utf8);

std::size_t WriteUnicodeCharacter(uint32_t code_point, std::string* output);

std::size_t WriteUnicodeCharacter(uint32_t code_point, std::wstring* output);

}  // namespace strings
}  // namespace kwc

#endif  // KWCTOOLKIT_STRINGS_UTF_STRING_CONVERSION_H_
