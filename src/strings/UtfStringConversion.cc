// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "strings/UtfStringConversion.h"

#include <icu_utf.h>

#include <cstddef>
#include <string>

#include "strings/StringUtils.h"

namespace kwc {
namespace strings {

namespace {
template <typename Char>
void PrepareForUTF8Output(const Char* src, std::size_t src_len, std::string* output) {
    output->clear();
    if (src_len == 0)
        return;
    if (src[0] < 0x80) {
        // assuming that the entire input will be ASCII
        output->reserve(src_len);
    } else {
        // assuming that the entire input is non-ASCII with 3 bytes per char
        output->reserve(src_len * 3);
    }
}

template <typename String>
void PrepareForUTF16Or32Output(const char* src, std::size_t src_len, String* output) {
    output->clear();
    if (src_len == 0)
        return;
    if (static_cast<unsigned char>(src[0]) < 0x80) {
        // assuming that the entire input is all ASCII
        output->reserve(src_len);
    } else {
        // assuming that the UTF-8 sequence has 2 bytes for each char
        output->reserve(src_len / 2);
    }
}

// Concrete template instantiations for common types
template void PrepareForUTF16Or32Output(const char*, std::size_t, std::wstring*);

bool IsValidCodepoint(uint32_t code_point) {
    // Excludes the surrogate code points ([0xD800, 0xDFFF]) and code points larger than 0x10FFFF
    // (the highest codepoint allowed). Non-characters and unassigned code points are allowed
    return code_point < 0xD800u || (code_point >= 0xE000u && code_point <= 0x10FFFFu);
}

bool ReadUnicodeCharacter(const char* src,
                          int32_t src_len,
                          int32_t* char_index,
                          uint32_t* code_point_out) {
    // U8_NEXT expects to be able to use -1 to signal an error, thus we must use a signed type for
    // code_point. However, this function returns false on error anyways, so code_point_out is
    // unsigned
    int32_t code_point;
    CBU8_NEXT(src, *char_index, src_len, code_point);
    *code_point_out = static_cast<uint32_t>(code_point);

    // the ICU macro above moves to the next char, but we want to point to the last char consumed
    (*char_index)--;

    // validate the decoded value
    return IsValidCodepoint(code_point);
}

bool ReadUnicodeCharacter(const wchar_t* src, int32_t, int32_t* char_index, uint32_t* code_point) {
    // conversion is straight-forward since the source is 32-bit.
    *code_point = src[*char_index];
    return IsValidCodepoint(*code_point);
}

// Converts the given source unicode character type to the given destination unicode character type
// as a STL std::string. The given input buffer and size determine the source and the given output
// STL std::string will be replaced by the result
template <typename SrcChar, typename DstString>
bool ConvertUnicode(const SrcChar* src, std::size_t src_len, DstString* output) {
    // ICU required 32-bit numbers
    bool success = true;
    auto src_len32 = static_cast<int32_t>(src_len);
    for (int32_t i = 0; i < src_len32; i++) {
        uint32_t code_point;
        if (ReadUnicodeCharacter(src, src_len32, &i, &code_point)) {
            WriteUnicodeCharacter(code_point, output);
        } else {
            WriteUnicodeCharacter(0xFFFD, output);
            success = false;
        }
    }

    return success;
}

}  // namespace

std::size_t WriteUnicodeCharacter(uint32_t code_point, std::string* output) {
    if (code_point <= 0x7f) {
        // fast path the common case of one byte
        output->push_back(static_cast<char>(code_point));
        return 1;
    }

    // CBU8_APPEND_UNSAFE can append up to 4 bytes
    std::size_t char_offset = output->length();
    std::size_t original_char_offset = char_offset;
    output->resize(char_offset + CBU8_MAX_LENGTH);

    CBU8_APPEND_UNSAFE(&(*output)[0], char_offset, code_point);

    // CBU8_APPEND_UNSAFE will advance our pointer past the inserted character,
    // so it will represent the new length of the string.
    output->resize(char_offset);
    return char_offset - original_char_offset;
}

std::size_t WriteUnicodeCharacter(uint32_t code_point, std::wstring* output) {
    output->push_back(code_point);
    return 1;
}

bool WideToUTF8(const wchar_t* src, std::size_t src_len, std::string* output) {
    if (IsStringASCII(std::wstring(src, src_len))) {
        output->assign(src, src + src_len);
        return true;
    } else {
        PrepareForUTF8Output(src, src_len, output);
        return ConvertUnicode(src, src_len, output);
    }
}

std::string WideToUTF8(const std::wstring& wide) {
    if (IsStringASCII(wide)) {
        return std::string(wide.data(), wide.data() + wide.length());
    }

    std::string ret;
    PrepareForUTF8Output(wide.data(), wide.length(), &ret);
    ConvertUnicode(wide.data(), wide.length(), &ret);
    return ret;
}

bool UTF8ToWide(const char* src, std::size_t src_len, std::wstring* output) {
    if (IsStringASCII(std::wstring(src, src + src_len))) {
        output->assign(src, src + src_len);
        return true;
    } else {
        PrepareForUTF16Or32Output(src, src_len, output);
        return ConvertUnicode(src, src_len, output);
    }
}

std::wstring UTF8ToWide(const std::string& utf8) {
    if (IsStringASCII(utf8)) {
        return std::wstring(utf8.begin(), utf8.end());
    }

    std::wstring ret;
    PrepareForUTF16Or32Output(utf8.data(), utf8.length(), &ret);
    ConvertUnicode(utf8.data(), utf8.length(), &ret);
    return ret;
}
}  // namespace strings
}  // namespace kwc