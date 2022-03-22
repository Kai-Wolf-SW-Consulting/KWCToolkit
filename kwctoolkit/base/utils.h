// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_UTILS_H_
#define KWCTOOLKIT_BASE_UTILS_H_

#include <cstddef>
#include <iterator>
#include <tuple>
#include <utility>

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
    struct iterator {  // NOLINT
        size_t i_;
        TIter iter_;
        bool operator!=(const iterator& other) const { return iter_ != other.iter_; }
        void operator++() {
            ++i_;
            ++iter_;
        }
        auto operator*() const { return std::tie(i_, *iter_); }
    };
    struct IterableWrapper {
        T iterable_;
        auto begin() { return iterator{0, std::begin(iterable_)}; }
        auto end() { return iterator{0, std::end(iterable_)}; }
    };
    return IterableWrapper{std::forward<T>(iterable)};
}

}  // namespace base
}  // namespace kwc

#endif  // KWCTOOLKIT_BASE_UTILS_H_
