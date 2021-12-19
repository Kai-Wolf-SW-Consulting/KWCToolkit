// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "base/CmdlineFlags.h"

#include <gtest/gtest.h>

#include "base/IntegralTypes.h"

using namespace kwc;

TEST(CmdLineFlagsTest, CheckUInt16Parsing) {
    uint16 val;
    ASSERT_TRUE(base::ParseUInt16Flag("--value=65535", "value", &val));
    EXPECT_EQ(val, 65535);
}