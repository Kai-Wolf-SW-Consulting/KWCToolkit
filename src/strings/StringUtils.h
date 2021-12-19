// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_STRINGS_STRING_UTILS_H_
#define KWCTOOLKIT_STRINGS_STRING_UTILS_H_

#include <algorithm>
#include <iosfwd>
#include <string>
#include <vector>

// String utility functions used throughout the codebase

namespace kwc {
namespace strings {

// Whitespace for char tab, line tab, form feed (FF) and space
#define WHITESPACE_ASCII_NO_CR_LF 0x09, 0x0B, 0x0C, 0x20

// Whitespace for line feed (LF) and carriage return (CR)
#define WHITESPACE_ASCII WHITESPACE_ASCII_NO_CR_LF, 0x0A, 0x0D

constexpr char kWhitespaceASCII[] = {WHITESPACE_ASCII, 0};

enum TrimPositions {
    TRIM_NONE = 0,
    TRIM_LEADING = 1 << 0,
    TRIM_TRAILING = 1 << 1,
    TRIM_ALL = TRIM_LEADING | TRIM_TRAILING,
};

inline bool IsWhitespace(const char ch) {
    return isspace(ch);
}

inline bool IsWhitespace(const char* ch) {
    return isspace(ch[0]);
}

bool IsStringASCII(const std::wstring& str);

bool IsStringASCII(const std::string& str);

template <typename Char>
bool IsASCIILower(const Char c) {
    return 'a' <= c && c <= 'z';
}

inline char ToLowerASCII(const char c) {
    return ('A' <= c && c <= 'Z') ? (c + ('a' - 'A')) : c;
}

std::string ToLowerASCII(const std::string& str);

template <typename Char>
bool IsASCIIUpper(Char c) {
    return 'A' <= c && c <= 'Z';
}

inline char ToUpperASCII(const char c) {
    return ('a' <= c && c <= 'z') ? (c + ('A' - 'a')) : c;
}

std::string ToUpperASCII(const std::string& str);

template <typename Char>
struct CaseInsensitiveCompare {
    bool operator()(Char x, Char y) const { return ToLowerASCII(x) == ToLowerASCII(y); }
};

std::string TrimString(const std::string& input,
                       const std::string& trim_chars,
                       TrimPositions positions);

template <typename String>
bool StartsWith(String str, String search_for, bool case_sensitive) {
    if (search_for.size() > str.size()) {
        return false;
    }

    auto source = str.substr(0, search_for.size());
    if (case_sensitive) {
        return source == search_for;
    }

    return std::equal(search_for.begin(), search_for.end(), source.begin(),
                      CaseInsensitiveCompare<typename String::value_type>());
}

template <typename String>
bool EndsWith(const String& value, const String& suffix) {
    if (suffix.size() > value.size()) {
        return false;
    }
    return std::equal(suffix.rbegin(), suffix.rend(), value.rbegin());
}

std::vector<float> StringsToFloat(const std::vector<std::string>& strings);

}  // namespace strings
}  // namespace kwc

#endif  // KWCTOOLKIT_STRINGS_STRING_UTILS_H_
