// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/system/Environment.h"

#include <gtest/gtest.h>

using namespace kwc::system;

TEST(EnvironmentTest, SetEnvironmentVariable) {
    std::unique_ptr<Environment> env(Environment::create());

    const char foobar_upper[] = "FOOBAR";
    const char foobar_lower[] = "foobar";
    EXPECT_TRUE(env->setEnvVar(foobar_upper, foobar_lower));
    EXPECT_TRUE(env->hasEnvVar(foobar_upper));

    std::string value;
    EXPECT_TRUE(env->getEnvVar(foobar_upper, &value));
    ASSERT_EQ(value, foobar_lower);
}

TEST(EnvironmentTest, HasEnvironmentVariable) {
    std::unique_ptr<Environment> env(Environment::create());
    ASSERT_TRUE(env->hasEnvVar("PATH"));
}

TEST(EnvironmentTest, UnsetEnvironmentVariable) {
    std::unique_ptr<Environment> env(Environment::create());

    const char foobar_upper[] = "FOOBAR";
    const char foobar_lower[] = "foobar";

    // first set the environment variable
    EXPECT_TRUE(env->setEnvVar(foobar_upper, foobar_lower));
    EXPECT_TRUE(env->hasEnvVar(foobar_upper));
    EXPECT_TRUE(env->unsetEnvVar(foobar_upper));
    ASSERT_FALSE(env->hasEnvVar(foobar_upper));
}

TEST(EnvironmentTest, GetReverseEnvironmentVariable) {
    std::unique_ptr<Environment> env(Environment::create());
    const char foobar_upper[] = "FOOBAR";
    const char foobar_lower[] = "foobar";

    // set variable in UPPER case
    EXPECT_TRUE(env->setEnvVar(foobar_upper, foobar_lower));

    // now try to get this variable passing the lower case
    std::string env_value;
    EXPECT_TRUE(env->getEnvVar(foobar_lower, &env_value));

    ASSERT_EQ(env_value, foobar_lower);
    EXPECT_TRUE(env->unsetEnvVar(foobar_upper));

    const char bar[] = "bar";
    // now for the opposite
    EXPECT_TRUE(env->setEnvVar(foobar_lower, bar));
    EXPECT_TRUE(env->getEnvVar(foobar_upper, &env_value));

    ASSERT_EQ(env_value, bar);
    EXPECT_TRUE(env->unsetEnvVar(foobar_lower));
}