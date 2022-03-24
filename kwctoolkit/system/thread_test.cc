// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/system/thread.h"

#include <gtest/gtest.h>

using namespace kwc::system;

namespace {
void NothingRunFunction(void*) {}

void SetBoolFlagRunFunction(void* obj) {
    bool* obj_as_bool = static_cast<bool*>(obj);
    *obj_as_bool = true;
}
}  // namespace

TEST(ThreadTest, CheckStartStop) {
    Thread thread(&NothingRunFunction, nullptr, "ThreadTest");
    EXPECT_TRUE(thread.name() == "ThreadTest");
    EXPECT_TRUE(thread.getThreadRef() == 0);
    thread.start();
    EXPECT_TRUE(thread.getThreadRef() != 0);
    thread.stop();
    EXPECT_TRUE(thread.getThreadRef() == 0);
}

TEST(ThreadTest, CheckStartStopWithTwoFuncs) {
    Thread thread1(&NothingRunFunction, nullptr, "ThreadTest1");
    Thread thread2(&NothingRunFunction, nullptr, "ThreadTest2");
    EXPECT_TRUE(thread1.getThreadRef() == thread2.getThreadRef());
    thread1.start();
    thread2.start();
    EXPECT_TRUE(thread1.getThreadRef() != thread2.getThreadRef());
    thread2.stop();
    thread1.stop();
}

TEST(ThreadTest, CheckRunFunctionCalled) {
    bool flag = false;
    Thread thread(&SetBoolFlagRunFunction, &flag, "RunFunctionCalled");
    thread.start();
    thread.stop();
    EXPECT_TRUE(flag);
}
