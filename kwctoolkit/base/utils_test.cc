// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/base/utils.h"

#include <gtest/gtest.h>

#include <vector>

using namespace kwc::base;

TEST(UtilsTest, EnumerateOverTuples) {
    std::vector<float> things{3.14f, 5.12f, 6.66f, 8.21f};

    int index = 0;
    for (auto [i, thing] : Enumerate(things)) {
        ASSERT_EQ(i, index);
        ASSERT_EQ(thing, things[index]);
        index++;
    }
    ASSERT_EQ(index, 4);
}

TEST(UtilsTest, IterateOverCStyleArrays) {
    const char *strings[] = {"foo", "bar", "bam", "bat"};
    size_t str_len = 4;

    int expected = 0;
    for (const auto str : Iterate(strings, str_len)) {
        EXPECT_STREQ(strings[expected++], str);
    }
}

TEST(UtilsTest, IterateOverSubrangePointers) {
    int numbers[] = {17, 23, 42, 36, 49, 85, 96};

    int expected = 1;
    int *num1 = &numbers[expected];
    int *num2 = &numbers[5];
    for (const auto num : Iterate(num1, num2)) {
        EXPECT_EQ(numbers[expected++], num);
    }
}

TEST(UtilsTest, MergeVariadicNumberOfVectors) {
    std::vector<float> a{1, 2, 3};
    std::vector<float> b{4, 5, 6};
    std::vector<float> c{7, 8, 9};

    const auto result = Merge(a, b, c);
    ASSERT_EQ(result.size(), 9);
    int expected = 1;
    for (const auto ele : result) {
        ASSERT_EQ(expected++, ele);
    }
}