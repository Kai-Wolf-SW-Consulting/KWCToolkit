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
unsigned int levenshtein_distance(const T s1, const T s2) {
    const auto len1 = s1.size();
    const auto len2 = s2.size();
    std::vector<unsigned int> col(len2 + 1);
    std::vector<unsigned int> prevCol(len2 + 1);

    std::iota(prevCol.begin(), prevCol.end(), 0);

    for (auto i = 0; i < len1; i++) {
        col[0] = i + 1;
        for (auto j = 0; j < len2; j++) {
            col[j + 1] =
                std::min({prevCol[1 + j] + 1, col[j] + 1, prevCol[j] + (s1[i] == s2[j] ? 0 : 1)});
        }
        col.swap(prevCol);
    }
    return prevCol[len2];
}

template <>
unsigned int levenshtein_distance<const char*>(const char* s1, const char* s2) {
    std::string string1{s1};
    std::string string2{s2};
    return levenshtein_distance(string1, string2);
}

}  // namespace utils
}  // namespace kwc

#endif  // KWCTOOLKIT_UTILS_LEVENNSHTEIN_H_
