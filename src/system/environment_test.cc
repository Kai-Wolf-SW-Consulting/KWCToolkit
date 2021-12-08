// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "system/environment.h"

#include <gtest/gtest.h>

using namespace kwc::system;

TEST(EnvironmentTest, SetEnvironmentVariable) {
    std::unique_ptr<Environment> env(Environment::Create());

    const char kFoobarUpper[] = "FOOBAR";
    const char kFoobarLower[] = "foobar";
    EXPECT_TRUE(env->SetEnvVar(kFoobarUpper, kFoobarLower));
    EXPECT_TRUE(env->HasEnvVar(kFoobarUpper));

    std::string value;
    EXPECT_TRUE(env->GetEnvVar(kFoobarUpper, &value));
    ASSERT_EQ(value, kFoobarLower);
}

TEST(EnvironmentTest, HasEnvironmentVariable) {
    std::unique_ptr<Environment> env(Environment::Create());
    ASSERT_TRUE(env->HasEnvVar("PATH"));
}

TEST(EnvironmentTest, UnsetEnvironmentVariable) {
    std::unique_ptr<Environment> env(Environment::Create());

    const char kFoobarUpper[] = "FOOBAR";
    const char kFoobarLower[] = "foobar";

    // first set the environment variable
    EXPECT_TRUE(env->SetEnvVar(kFoobarUpper, kFoobarLower));
    EXPECT_TRUE(env->HasEnvVar(kFoobarUpper));
    EXPECT_TRUE(env->UnsetEnvVar(kFoobarUpper));
    ASSERT_FALSE(env->HasEnvVar(kFoobarUpper));
}

TEST(EnvironmentTest, GetReverseEnvironmentVariable) {
    std::unique_ptr<Environment> env(Environment::Create());
    const char kFoobarUpper[] = "FOOBAR";
    const char kFoobarLower[] = "foobar";

    // set variable in UPPER case
    EXPECT_TRUE(env->SetEnvVar(kFoobarUpper, kFoobarLower));

    // now try to get this variable passing the lower case
    std::string env_value;
    EXPECT_TRUE(env->GetEnvVar(kFoobarLower, &env_value));

    ASSERT_EQ(env_value, kFoobarLower);
    EXPECT_TRUE(env->UnsetEnvVar(kFoobarUpper));

    const char kBar[] = "bar";
    // now for the opposite
    EXPECT_TRUE(env->SetEnvVar(kFoobarLower, kBar));
    EXPECT_TRUE(env->GetEnvVar(kFoobarUpper, &env_value));

    ASSERT_EQ(env_value, kBar);
    EXPECT_TRUE(env->UnsetEnvVar(kFoobarLower));
}