// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include <gtest/gtest.h>

#include <kwctoolkit/system/Thread.h>

using namespace kwc::system;

namespace {
void nothingRunFunction(void*) {}

void setBoolFlagRunFunction(void* obj) {
    bool* objAsBool = static_cast<bool*>(obj);
    *objAsBool = true;
}
}  // namespace

TEST(ThreadTest, CheckStartStop) {
    Thread thread(&nothingRunFunction, nullptr, "ThreadTest");
    EXPECT_TRUE(thread.name() == "ThreadTest");
    EXPECT_TRUE(thread.getThreadRef() == 0);
    thread.start();
    EXPECT_TRUE(thread.getThreadRef() != 0);
    thread.stop();
    EXPECT_TRUE(thread.getThreadRef() == 0);
}

TEST(ThreadTest, CheckStartStopWithTwoFuncs) {
    Thread thread1(&nothingRunFunction, nullptr, "ThreadTest1");
    Thread thread2(&nothingRunFunction, nullptr, "ThreadTest2");
    EXPECT_TRUE(thread1.getThreadRef() == thread2.getThreadRef());
    thread1.start();
    thread2.start();
    EXPECT_TRUE(thread1.getThreadRef() != thread2.getThreadRef());
    thread2.stop();
    thread1.stop();
}

TEST(ThreadTest, CheckRunFunctionCalled) {
    bool flag = false;
    Thread thread(&setBoolFlagRunFunction, &flag, "RunFunctionCalled");
    thread.start();
    thread.stop();
    EXPECT_TRUE(flag);
}