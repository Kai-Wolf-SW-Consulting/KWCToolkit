// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/utils/Regex.h"

#include <gtest/gtest.h>

#include <regex>
#include <string>

using namespace kwc::utils;

TEST(RegexTest, matchAlternativesFalse) {
    Regex regex = Regex().anythingBut("foobar").alternative("string");

    std::string test{"foobar"};

    ASSERT_FALSE(regex.match(test));
}

TEST(RegexTest, matchRegexWithURL) {
    Regex regex = Regex()
                      .startOfLine("http")
                      .maybe("s")
                      .then("://")
                      .somethingBut("temp")
                      .anything()
                      .anythingBut(" ")
                      .withoutCaseSensitivity();

    std::string test{"Http://www.example.com"};

    ASSERT_TRUE(regex.match(test));
}

TEST(RegexTest, searchManyResultsInString) {
    Regex regex = Regex().find("foobar").alternative("string");
    std::string test("There should be a foobar in this string, please find it!");

    ASSERT_TRUE(regex.search(test));
}
