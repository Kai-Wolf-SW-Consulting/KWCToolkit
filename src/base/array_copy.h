// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_ARRAY_COPY_H_
#define KWCTOOLKIT_BASE_ARRAY_COPY_H_

#include <cstddef>
#include <cstring>
#include <iostream>
#include <type_traits>

namespace kwc {

template <typename T, std::size_t n>
inline void ArrayCopy(T (&dst)[n], T (&src)[n]) {
    static_assert(std::is_trivially_copy_assignable<T>::value,
                  "element type has non-trivial copy assignment");
    std::memcpy(dst, src, sizeof(T) * n);
}

template <typename T, std::size_t m, std::size_t n>
inline void ArrayCopy(T (&dst)[m], T (&src)[n]) {
    static_assert(m >= n, "destination too small");
    for (std::size_t idx = 0; idx < n; ++idx) {
        dst[idx] = src[idx];
    }
}

}  // namespace kwc

#endif  // KWCTOOLKIT_BASE_ARRAY_COPY_H_
