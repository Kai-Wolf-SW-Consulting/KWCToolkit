// Copyright (c) 2022, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_IMAGE_IMAGE_UTILS_H
#define KWCTOOLKIT_IMAGE_IMAGE_UTILS_H

#include <cstddef>

#include "kwctoolkit/base/integral_types.h"

namespace kwc {
namespace image {

bool CheckSizeArgsOverflow(uint64 stride, std::size_t height);

}  // namespace image
}  // namespace kwc

#endif  // KWCTOOLKIT_IMAGE_IMAGE_UTILS_H
