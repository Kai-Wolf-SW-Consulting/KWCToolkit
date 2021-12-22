// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include <gtest/gtest.h>

#include <kwctoolkit/system/SystemInfo.h>

using namespace kwc::system;

TEST(SystemInfoTest, GetNumberOfCPUs) {
    // Check for a sane value only
    EXPECT_GE(SystemInfo::getNumberOfCPUs(), 1);
}

TEST(SystemInfoTest, GetAmountOfMemory) {
    // Simple sanity checks, virtual mem may be zero which means unlimited memory
    EXPECT_GT(SystemInfo::getAmountOfPhysicalMemory(), 0);
    EXPECT_GT(SystemInfo::getAmountOfPhysicalMemoryMB(), 0);
    EXPECT_GE(SystemInfo::getAmountOfVirtualMemory(), 0);
    EXPECT_GE(SystemInfo::getAmountOfVirtualMemoryMB(), 0);
}