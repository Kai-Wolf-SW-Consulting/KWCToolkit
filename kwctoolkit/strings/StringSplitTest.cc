// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/strings/StringSplit.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <string>
#include <vector>

using namespace kwc::strings;
using ::testing::ElementsAre;

TEST(StringSplitTest, SplitAtDelimiter) {
    const auto result = SplitString("this;seems;like;csv", ";");
    ASSERT_EQ(4, result.size());
    EXPECT_THAT(result, ElementsAre("this", "seems", "like", "csv"));
}

TEST(StringSplitTest, SplitWithEmptyString) {
    const auto result = SplitString(std::string(), "DELIM");
    ASSERT_EQ(0, result.size());
    EXPECT_THAT(result, ElementsAre());
}

TEST(StringSplitTest, SplitWithEmptyDelimiter) {
    const auto result = SplitString("ThisIsMyString", std::string());
    ASSERT_EQ(1, result.size());
    EXPECT_THAT(result, ElementsAre("ThisIsMyString"));
}

TEST(StringSplitTest, SplitWithoutDelimiter) {
    const auto result = SplitString("herecomesafancylongword", "DELIM");
    ASSERT_EQ(1, result.size());
}
