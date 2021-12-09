// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "system/aligned_alloc.h"

#include <gtest/gtest.h>

using namespace kwc::system;

struct AlignedAllocTestFixture : public ::testing::TestWithParam<int> {};

TEST_P(AlignedAllocTestFixture, GetAlignedMemory) {
    auto size = GetParam();
    auto* ptr = reinterpret_cast<float*>(alignedAlloc(size * sizeof(float)));
    ASSERT_TRUE(ptr != nullptr);
    alignedFree(ptr);
}

TEST_P(AlignedAllocTestFixture, AccessAlignedMemory) {
    auto size = GetParam();
    auto* ptr = reinterpret_cast<float*>(alignedAlloc(size * sizeof(float)));
    for (int idx = 0; idx < size; ++idx) {
        ASSERT_NO_FATAL_FAILURE(ptr[idx] = 0.0f);
    }

    for (int idx = 0; idx < size; ++idx) {
        ASSERT_EQ(ptr[idx], 0.0f);
    }

    alignedFree(ptr);
}

INSTANTIATE_TEST_CASE_P(
    AlignedAllocTests,
    AlignedAllocTestFixture,
    ::testing::Values(1, 2, 3, 5, 7, 11, 32, 64, 128, 256, 512, 1024, 2048, 4096));
