// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "base/RefCount.h"

#include <gtest/gtest.h>

#include <thread>

#include "base/IntegralTypes.h"

using namespace kwc;
using namespace kwc::base;

struct RefCountTester : public RefCount {
    RefCountTester() : RefCount() {}

    RefCountTester(uint64 payload) : RefCount(payload) {}

    RefCountTester(bool* deleted) : deleted(deleted) {}

    ~RefCountTester() override {
        if (deleted != nullptr) {
            *deleted = true;
        }
    }

    RefCountTester* getThis() { return this; }

    bool* deleted = nullptr;
};

TEST(RefCountTest, StartsWithOneRefCount) {
    auto deleted = false;
    auto test = new RefCountTester(&deleted);

    test->release();
    ASSERT_TRUE(deleted);
}

TEST(RefCountTest, AddingRefsDoesNotDelete) {
    auto deleted = false;
    auto test = new RefCountTester(&deleted);

    test->reference();
    test->release();
    ASSERT_FALSE(deleted);

    test->release();
    ASSERT_TRUE(deleted);
}

TEST(RefCountTest, CheckNoRaceConditions) {
    auto deleted = false;
    auto* test = new RefCountTester(&deleted);

    auto refManyTimes = [test]() {
        for (int i = 0; i < 100000; i++) {
            test->reference();
        }
    };

    std::thread t1(refManyTimes);
    std::thread t2(refManyTimes);

    t1.join();
    t2.join();
    ASSERT_EQ(test->getRefCountForTesting(), 200001);

    auto releaseManyTimes = [test]() {
        for (int i = 0; i < 100000; i++) {
            test->release();
        }
    };

    std::thread t3(releaseManyTimes);
    std::thread t4(releaseManyTimes);
    t3.join();
    t4.join();
    ASSERT_EQ(test->getRefCountForTesting(), 1);
    test->release();
}

TEST(RefCountTest, CheckRemoveRefAfterScope) {
    auto deleted = false;
    {
        Ref<RefCountTester> test(new RefCountTester(&deleted));
        test->release();
    }
    ASSERT_TRUE(deleted);
}

TEST(RefCountTest, CheckGettingPointerOfRef) {
    auto* original = new RefCountTester;
    Ref<RefCountTester> test(original);
    test->release();

    ASSERT_EQ(test.get(), original);
    ASSERT_EQ(test->getThis(), original);
}

TEST(RefCountTest, CheckRefDefaultsToNull) {
    Ref<RefCountTester> test;

    ASSERT_EQ(test.get(), nullptr);
    ASSERT_EQ(test->getThis(), nullptr);
}

TEST(RefCountTest, CheckBoolConversion) {
    Ref<RefCountTester> empty;
    Ref<RefCountTester> full(new RefCountTester);
    full->release();

    if (!full || empty) {
        ASSERT_TRUE(false);
    }
}

TEST(RefCountTest, CheckCopyConstruction) {
    bool deleted = false;
    RefCountTester* original = new RefCountTester(&deleted);

    Ref<RefCountTester> source(original);
    Ref<RefCountTester> dest(source);
    original->release();

    ASSERT_EQ(source.get(), original);
    ASSERT_EQ(dest.get(), original);

    source = nullptr;
    ASSERT_FALSE(deleted);
    dest = nullptr;
    ASSERT_TRUE(deleted);
}

TEST(RefCountTest, CheckInitialPayloadValue) {
    auto* test = new RefCountTester();
    ASSERT_EQ(test->getRefCountPayload(), 0);
    test->release();

    auto* test2 = new RefCountTester(uint64(0));
    ASSERT_EQ(test2->getRefCountPayload(), 0);
    test2->release();

    auto* test3 = new RefCountTester(1);
    ASSERT_EQ(test3->getRefCountPayload(), 1);
    test3->release();
}

TEST(RefCountTest, CheckPayloadSameAfterRefRelease) {
    auto* test = new RefCountTester(1);
    ASSERT_EQ(test->getRefCountPayload(), 1);

    test->reference();
    ASSERT_EQ(test->getRefCountPayload(), 1);
    test->release();
    ASSERT_EQ(test->getRefCountPayload(), 1);

    test->release();
}
