// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_ARRAY_SIZE_H_
#define KWCTOOLKIT_BASE_ARRAY_SIZE_H_

#include <cstddef>

namespace kwc {
namespace base {
namespace internal {

// The arraysize(arr) macro returns the number of elements in array.
// The expression is a compile-time constant, and therefore can be used in
// defining new arrays, for example. If you use arraysize on a pointer by
// mistake, you will get a compile-time error, since *foo_ and foo_[] are
// two different things.
template <typename T, std::size_t N>
char (&ArraySizeHelper(T (&array)[N]))[N];

#define arraysize(arr) (sizeof(::kwc::base::internal::ArraySizeHelper(arr)))
}  // namespace internal

// Used to explicitly mark the return value of a function as unused.
template <typename T>
inline void IgnoreResult(const T& /*unused*/) {}
}  // namespace base
}  // namespace kwc

#endif  // KWCTOOLKIT_BASE_ARRAY_SIZE_H_
