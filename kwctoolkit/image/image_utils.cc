// Copyright (c) 2022, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/image/image_utils.h"

namespace kwc {
namespace image {

bool CheckSizeArgsOverflow(uint64 stride, std::size_t height) {
    const uint64 total_size = stride * height;
    int ok = (total_size == (size_t)total_size);
    // Ensure that 'stride' fits into integer
    ok = ok && ((uint64)(int)stride == stride);
    return !!ok;
}

}  // namespace image
}  // namespace kwc