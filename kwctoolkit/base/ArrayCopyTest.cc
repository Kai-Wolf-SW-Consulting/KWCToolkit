// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/base/ArrayCopy.h"

#include <gtest/gtest.h>

TEST(ArrayCopyTest, CopyElementWiseIntoEqualSizedArray) {
    int a[] = {17, 23, 42, 69, 53, 72, 96};
    int b[7];
    kwc::ArrayCopy(b, a);

    for (std::size_t idx = 0; idx < 7; idx++) {
        ASSERT_EQ(a[idx], b[idx]);
    }
}
