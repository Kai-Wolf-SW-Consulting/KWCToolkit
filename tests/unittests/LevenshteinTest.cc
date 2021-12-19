// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "utils/Levenshtein.h"

#include <gtest/gtest.h>

#include <string>
#include <vector>

using namespace kwc::utils;

TEST(LevenshteinTest, SameStringGivesZeroDistance) {
    const std::string lev1("FooBar");
    ASSERT_TRUE(levenshtein_distance(lev1, lev1) == 0);
}

TEST(LevenshteinTest, SameStringWithCaseSensitivity) {
    const std::string lev1("FooBar");
    const std::string lev2("foobar");
    ASSERT_FALSE(levenshtein_distance(lev1, lev2) == 0);
}

TEST(LevenshteinTest, TwoStringsDifferInOnCharGiveDistOne) {
    const std::string lev1("foobar");
    const std::string lev2("foobat");
    ASSERT_TRUE(levenshtein_distance(lev1, lev2) == 1);
}

TEST(LevenshteinTest, AcceptsCStyleStrings) {
    const char* lev1 = "FooBar";
    ASSERT_TRUE(levenshtein_distance(lev1, lev1) == 0);
}

TEST(LevenshteinTest, AcceptsVectorCharInput) {
    const std::vector<char> lev1{'F', 'o', 'o', 'B', 'a', 'r'};
    const std::vector<char> lev2{'F', 'o', 'o', 'B', 'u', 't'};
    ASSERT_TRUE(levenshtein_distance(lev1, lev2) == 2);
}

TEST(LevenshteinTest, SatisfiesAxiomsOfMetricSpace) {
    const std::string lev1("Foo");
    const std::string lev2("Bar");
    // identity of indiscernibles
    EXPECT_GT(levenshtein_distance(lev1, lev2), 0);
    EXPECT_EQ(levenshtein_distance(lev1, lev1), 0);
    // symmetry
    EXPECT_EQ(levenshtein_distance(lev1, lev2), levenshtein_distance(lev2, lev1));
    // triangle inequality
    const std::string lev3("Bat");
    auto d_xy = levenshtein_distance(lev1, lev2);
    auto d_xz = levenshtein_distance(lev1, lev3);
    auto d_zy = levenshtein_distance(lev3, lev2);
    EXPECT_LE(d_xy, d_xz + d_zy);
}
