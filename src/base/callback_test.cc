// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "base/callback.h"

#include <gtest/gtest.h>

#include "base/callback_types.h"

namespace {
int value = 0;

int FreeFunctionWithReturnParam() {
    value += 17;
    return value;
}
}  // namespace

TEST(CallbackTest, CallbackWithFreeFunction) {
    auto cb = kwc::MakeCallback(&FreeFunctionWithReturnParam);
    ASSERT_TRUE(value == 0);
    ASSERT_TRUE(cb->run() == 17);
}

TEST(CallbackTest, CheckForOnetimeCallback) {
    auto cb = kwc::MakeCallback(&FreeFunctionWithReturnParam);
    ASSERT_FALSE(cb->isRepeatable());
}

TEST(CallbackTest, CheckForMultitimeCallback) {
    auto cb = kwc::MakePermanentCallback(&FreeFunctionWithReturnParam);
    ASSERT_TRUE(cb->isRepeatable());
}