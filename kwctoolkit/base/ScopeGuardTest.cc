// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/base/ScopeGuard.h"

#include <gtest/gtest.h>

TEST(ScopeGuardTest, ExecuteScopeGuardAfterScope) {
    using namespace kwc::base;
    int foo = 17;
    {
        KWC_SCOPE_EXIT { foo = 23; };
    }
    ASSERT_EQ(foo, 23);
}