// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_FOR_EACH_ARGUMENT_H_
#define KWCTOOLKIT_BASE_FOR_EACH_ARGUMENT_H_

#include <utility>

namespace kwc {

template <class F, class... Args>
void ForEachArgument(F f, Args&&... args) {
    [](...) {}((f(std::forward<Args>(args)), 0)...);
}

template <class F, class... Args>
void ForEachArgumentInOrder(F f, Args&&... args) {
    (void)(int[]){(f(std::forward<Args>(args)), 0)...};
}

}  // namespace kwc

#endif  // KWCTOOLKIT_BASE_FOR_EACH_ARGUMENT_H_
