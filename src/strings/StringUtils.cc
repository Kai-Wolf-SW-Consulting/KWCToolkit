// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "strings/StringUtils.h"

#include <stdint.h>

#include <cstddef>

namespace kwc {
namespace strings {

namespace {

// Assuming that a pointer is the size of a *machine word*, then uintptr_t is an integer type that
// is also of machine word size
using MachineWord = uintptr_t;
const uintptr_t kMachineWordAlignmentMask = sizeof(MachineWord) - 1;

inline bool IsAlignedToMachineWord(const void* pointer) {
    return !(reinterpret_cast<MachineWord>(pointer) & kMachineWordAlignmentMask);
}

template <typename T>
inline T* AlignToMachineWord(T* pointer) {
    return reinterpret_cast<T*>(reinterpret_cast<MachineWord>(pointer) &
                                ~kMachineWordAlignmentMask);
}

template <std::size_t size, typename CharType>
struct NonASCIIMask;

template <>
struct NonASCIIMask<4, char> {
    static inline uint32_t value() { return 0x80808080U; }
};

template <>
struct NonASCIIMask<8, char> {
    static inline uint64_t value() { return 0x8080808080808080ULL; }
};

template <>
struct NonASCIIMask<4, wchar_t> {
    static inline uint32_t value() { return 0xFFFFFF80U; }
};

template <>
struct NonASCIIMask<8, wchar_t> {
    static inline uint64_t value() { return 0xFFFFFF80FFFFFF80ULL; }
};

template <typename Char>
inline bool IsStringASCIIImpl(const Char* chars, std::size_t length) {
    MachineWord all_char_bits = 0;
    const Char* end = chars + length;

    // align the input
    while (!IsAlignedToMachineWord(chars) && chars != end) {
        all_char_bits |= *chars;
        ++chars;
    }

    // compare the values of CPU word size
    const Char* word_end = AlignToMachineWord(end);
    const std::size_t loop_increment = sizeof(MachineWord) / sizeof(Char);
    while (chars != word_end) {
        all_char_bits |= *chars;
        chars += loop_increment;
    }

    // process the remaining bytes
    while (chars != end) {
        all_char_bits |= *chars;
        ++chars;
    }

    MachineWord non_ascii_bit_mask = NonASCIIMask<sizeof(MachineWord), Char>::value();
    return !(all_char_bits & non_ascii_bit_mask);
}

template <typename StringType>
StringType ToLowerASCIIImpl(StringType str) {
    StringType ret;
    ret.reserve(str.size());
    for (std::size_t idx = 0; idx < str.size(); idx++) {
        ret.push_back(ToLowerASCII(str[idx]));
    }
    return ret;
}

template <typename StringType>
StringType ToUpperASCIIImpl(StringType str) {
    StringType ret;
    ret.reserve(str.size());
    for (std::size_t idx = 0; idx < str.size(); idx++) {
        ret.push_back(ToUpperASCII(str[idx]));
    }
    return ret;
}

}  // namespace

bool IsStringASCII(const std::wstring& str) {
    return IsStringASCIIImpl(str.data(), str.length());
}

bool IsStringASCII(const std::string& str) {
    return IsStringASCIIImpl(str.data(), str.length());
}

std::string ToLowerASCII(const std::string& str) {
    return ToLowerASCIIImpl<std::string>(str);
}

std::string ToUpperASCII(const std::string& str) {
    return ToUpperASCIIImpl<std::string>(str);
}

std::string TrimString(const std::string& input,
                       const std::string& trim_chars,
                       TrimPositions positions) {
    if (input.empty()) {
        return "";
    }
    auto begin = (positions & TRIM_LEADING) ? input.find_first_not_of(trim_chars) : 0;
    auto end = (positions & TRIM_TRAILING) ? input.find_last_not_of(trim_chars) + 1 : input.size();
    return input.substr(begin, end - begin);
}

std::vector<float> StringsToFloat(const std::vector<std::string>& strings) {
    std::vector<float> floats;
    for (const auto& ele : strings) {
        floats.push_back(std::stof(ele));
    }
    return floats;
}

}  // namespace strings
}  // namespace kwc
