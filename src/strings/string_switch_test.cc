// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "strings/string_switch.h"

#include <gtest/gtest.h>

#include <cstring>

using namespace kwc::strings;

TEST(StringSwitchTest, TestWithStdString) {
    const auto result = StringSwitch<std::string>("Foo")
                            .Case("Foo", "foo")
                            .Case("Bar", "bar")
                            .Case("Bat", "bat")
                            .Default("generic");
    EXPECT_EQ("foo", result);
}

TEST(StringSwitchTest, TestWithConstChar) {
    const auto result =
        StringSwitch<const char*>("value")
            .Case("value", "17")
            .Case("other", "23")
            .Default("42");
    ASSERT_TRUE(strncmp("17", result, 2) == 0);
}

TEST(StringSwitchTest, TestWithIntegers) {
    const auto result = StringSwitch<int>("value")
                            .Case("value", 17)
                            .Case("other", 23)
                            .Case("other", 36)
                            .Default(42);
    EXPECT_EQ(17, result);
}

TEST(StringSwitchTest, TestWithFloats) {
    const auto result = StringSwitch<float>("a")
                            .Case("a", 0.0F)
                            .Case("b", 1.0F)
                            .Case("c", -1.0F)
                            .Case("d", -0.0F)
                            .Default(2.0F);
    EXPECT_EQ(result, 0.0F);
}

TEST(StringSwitchTest, TestWithDefaultValue) {
    const auto result = StringSwitch<float>("e")
                            .Case("a", 0.0F)
                            .Case("b", 1.0F)
                            .Case("c", -1.0F)
                            .Case("d", -0.0F)
                            .Default(2.0F);
    EXPECT_EQ(result, 2.0F);
}