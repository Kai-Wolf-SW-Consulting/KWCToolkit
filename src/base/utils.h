// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWC_BASE_UTILS_H_
#define KWC_BASE_UTILS_H_

#include <tuple>
#include <vector>

namespace kwc {
namespace base {

// Base case for variadic template 'Merge'
template <typename T>
T Merge(const T& v) {
    return v;
}

// Merges a variadic number of T's into one container
template <typename T, typename... Args>
T Merge(T first, Args... args) {
    const auto next = Merge(args...);
    first.insert(first.end(), next.begin(), next.end());
    return first;
}

template <typename T,
          typename TIter = decltype(std::begin(std::declval<T>())),
          typename = decltype(std::end(std::declval<T>()))>
constexpr auto Enumerate(T&& iterable) {
    struct iterator {
        size_t i;
        TIter iter;
        bool operator!=(const iterator& other) const { return iter != other.iter; }
        void operator++() {
            ++i;
            ++iter;
        }
        auto operator*() const { return std::tie(i, *iter); }
    };
    struct iterable_wrapper {
        T iterable;
        auto begin() { return iterator{0, std::begin(iterable)}; }
        auto end() { return iterator{0, std::end(iterable)}; }
    };
    return iterable_wrapper{std::forward<T>(iterable)};
}

}  // namespace base
}  // namespace kwc

#endif  // KWC_BASE_UTILS_H_
