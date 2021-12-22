// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/base/ArraySize.h"

#include <gtest/gtest.h>

TEST(ArraySizeTest, GetCompileTimeArraySize) {
    int foobar[] = {2, 3, 5, 7, 9, 11};
    EXPECT_EQ(6, arraysize(foobar));
}