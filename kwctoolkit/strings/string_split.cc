// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/strings/string_split.h"

#include <cstddef>

#include "kwctoolkit/strings/string_utils.h"

namespace kwc {
namespace strings {

std::vector<std::string> SplitString(const std::string& input,
                                     const std::string& separator,
                                     WhitespaceHandling whitespace,
                                     SplitResult split_result) {
    std::vector<std::string> result;
    if (input.empty()) {
        return result;
    }

    std::size_t start = 0;
    while (start != std::string::npos) {
        auto end = input.find_first_of(separator, start);

        std::string piece;
        if (end == std::string::npos) {
            piece = input.substr(start);
            start = std::string::npos;
        } else {
            piece = input.substr(start, end - start);
            start = end + 1;
        }

        if (whitespace == WHITESPACE_TRIM) {
            piece = TrimString(piece, kWhitespaceASCII, TRIM_ALL);
        }

        if (split_result == SPLIT_WANT_ALL || !piece.empty()) {
            result.emplace_back(piece);
        }
    }

    return result;
}

}  // namespace strings
}  // namespace kwc
