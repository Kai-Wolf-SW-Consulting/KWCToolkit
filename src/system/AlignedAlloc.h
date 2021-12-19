// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_SYSTEM_ALIGNED_ALLOC_H_
#define KWCTOOLKIT_SYSTEM_ALIGNED_ALLOC_H_

#include <cstddef>

#include "base/Compiler.h"

namespace kwc {
namespace system {

// Minimum alignment for memory blocks
//  - This value needs to be at least the alignment for any integral data type
//  - For performance this should also be at least the size of L1/L2 cache line size to prevent
//    possible cache side effects.
//  - Depending on the context, it is useful that this is at least the size of a SIMD register,
//    as some instruction sets have at least performance differences or possibly different other
//    requirements based on that
constexpr std::ptrdiff_t kMinBlockAlignment = CACHELINE_SIZE;

// Platform-independent dispatcher for receiving aligned memory from the OS
void* alignedAlloc(std::ptrdiff_t num_bytes, std::size_t alignment = kMinBlockAlignment);

// Free memory with previously called alignedAlloc()
void alignedFree(void* ptr);

}  // namespace system
}  // namespace kwc

#endif  // KWCTOOLKIT_SYSTEM_ALIGNED_ALLOC_H_
