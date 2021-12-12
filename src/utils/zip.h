// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_UTILS_ZIP_H_
#define KWCTOOLKIT_UTILS_ZIP_H_

#include <algorithm>
#include <utility>
#include <vector>

namespace kwc {
namespace utils {

namespace internal {
template <typename S, typename T, typename F>
inline void transform(const std::vector<S>& as,
                      const std::vector<T>& bs,
                      std::vector<std::pair<S, T>>& rs,
                      F f) {
    std::transform(as.cbegin(), as.cend(), bs.cbegin(), rs.begin(), f);
}
}  // namespace internal

template <typename S, typename T>
std::vector<std::pair<S, T>> zip(const std::vector<S>& xs, const std::vector<T>& ys) {
    const auto xs_size = xs.size();
    const auto ys_size = ys.size();
    std::vector<std::pair<S, T>> r(std::min(xs_size, ys_size));
    if (xs_size <= ys_size) {
        internal::transform(xs, ys, r,
                            [](const S& e1, const T& e2) { return std::make_pair(e1, e2); });
    } else {
        internal::transform(ys, xs, r,
                            [](const T& e1, const S& e2) { return std::make_pair(e2, e1); });
    }
    return r;
}

}  // namespace utils
}  // namespace kwc

#endif  // KWCTOOLKIT_UTILS_ZIP_H_
