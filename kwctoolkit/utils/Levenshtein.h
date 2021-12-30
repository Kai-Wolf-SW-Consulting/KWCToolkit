// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_UTILS_LEVENSHTEIN_H_
#define KWCTOOLKIT_UTILS_LEVENSHTEIN_H_

#include <algorithm>
#include <numeric>
#include <string>
#include <vector>

namespace kwc {
namespace utils {

template <typename T>
unsigned int LevenshteinDistance(const T s1, const T s2) {
    const auto len1 = s1.size();
    const auto len2 = s2.size();
    std::vector<unsigned int> col(len2 + 1);
    std::vector<unsigned int> prev_col(len2 + 1);

    std::iota(prev_col.begin(), prev_col.end(), 0);

    for (auto i = 0; i < len1; i++) {
        col[0] = i + 1;
        for (auto j = 0; j < len2; j++) {
            col[j + 1] =
                std::min({prev_col[1 + j] + 1, col[j] + 1, prev_col[j] + (s1[i] == s2[j] ? 0 : 1)});
        }
        col.swap(prev_col);
    }
    return prev_col[len2];
}

template <>
unsigned int LevenshteinDistance<const char*>(const char* s1, const char* s2) {
    std::string string1{s1};
    std::string string2{s2};
    return LevenshteinDistance(string1, string2);
}

}  // namespace utils
}  // namespace kwc

#endif  // KWCTOOLKIT_UTILS_LEVENSHTEIN_H_
