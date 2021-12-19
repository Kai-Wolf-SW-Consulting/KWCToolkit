// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "system/AlignedAlloc.h"

#include <cstdlib>

#include "base/Platform.h"  // IWYU pragma: keep

namespace kwc {
namespace system {

void* alignedAlloc(std::ptrdiff_t num_bytes, std::size_t alignment) {
#if defined(OS_WINDOWS)
    return _aligned_malloc(num_bytes, alignment);
#else
    void* ptr;
    if (posix_memalign(&ptr, alignment, num_bytes)) {
        return nullptr;
    }
    return ptr;
#endif
}

void alignedFree(void* ptr) {
#if defined(OS_WINDOWS)
    _aligned_free(ptr);
#else
    free(ptr);
#endif
}

}  // namespace system
}  // namespace kwc
