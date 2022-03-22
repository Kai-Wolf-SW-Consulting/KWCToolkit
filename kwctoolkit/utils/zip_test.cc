// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/utils/zip.h"

#include <gtest/gtest.h>

#include <vector>

using namespace kwc::utils;

TEST(ZipTest, ZipStdIntVectors) {
    std::vector<int> as = {1, 3, 5, 7, 9};
    std::vector<int> bs = {2, 4, 6, 8, 10};

    const auto ab = Zip(as, bs);
    ASSERT_EQ(ab.size(), 5);
    ASSERT_EQ(ab[0], std::make_pair(1, 2));
    ASSERT_EQ(ab[1], std::make_pair(3, 4));
    ASSERT_EQ(ab[2], std::make_pair(5, 6));
    ASSERT_EQ(ab[3], std::make_pair(7, 8));
    ASSERT_EQ(ab[4], std::make_pair(9, 10));
}

TEST(ZipTest, ZipDifferentVectorSizes) {
    std::vector<int> as = {2, 3, 5, 7, 11, 13};
    std::vector<int> bs = {23, 42};

    const auto ab = Zip(as, bs);
    ASSERT_EQ(ab.size(), 2);
    ASSERT_EQ(ab[0], std::make_pair(2, 23));
    ASSERT_EQ(ab[1], std::make_pair(3, 42));
}