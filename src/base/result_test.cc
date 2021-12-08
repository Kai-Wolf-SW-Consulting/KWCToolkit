// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "base/result.h"

#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "base/integral_types.h"
#include "base/ref_count.h"

using namespace kwc;
using namespace kwc::base;

namespace {
int testError = 17;
float testSuccess = 23.0f;

class RefClass : public RefCount {
  public:
    int foo = 0;
};
}  // namespace

TEST(ResultTest, CreateErrorCaseFromVoid) {
    Result<void, int> result(std::make_unique<int>(testError));
    EXPECT_TRUE(result.isError());
    EXPECT_FALSE(result.isSuccess());

    auto stored = result.getError();
    EXPECT_EQ(*stored, testError);
}

TEST(ResultTest, CreateErrorCaseFromPointer) {
    Result<float*, int> result(std::make_unique<int>(testError));
    EXPECT_TRUE(result.isError());
    EXPECT_FALSE(result.isSuccess());

    auto stored = result.getError();
    EXPECT_EQ(*stored, testError);
}

TEST(ResultTest, CreateErrorCaseFromRef) {
    Result<Ref<RefClass>, int> result(std::make_unique<int>(testError));
    EXPECT_TRUE(result.isError());
    EXPECT_FALSE(result.isSuccess());

    auto stored = result.getError();
    EXPECT_EQ(*stored, testError);
}

TEST(ResultTest, CreateErrorCaseFromVector) {
    Result<std::vector<float>, int> result(std::make_unique<int>(testError));
    EXPECT_TRUE(result.isError());
    EXPECT_FALSE(result.isSuccess());

    auto stored = result.getError();
    EXPECT_EQ(*stored, testError);
}

TEST(ResultTest, MoveErrorCaseFromVoid) {
    Result<void, int> result(std::make_unique<int>(testError));
    Result<void, int> movedResult(std::move(result));

    EXPECT_TRUE(movedResult.isError());
    EXPECT_FALSE(movedResult.isSuccess());

    auto stored = movedResult.getError();
    EXPECT_EQ(*stored, testError);
}

TEST(ResultTest, MoveErrorCaseFromPointer) {
    Result<float*, int> result(std::make_unique<int>(testError));
    Result<float*, int> moved(std::move(result));

    EXPECT_TRUE(moved.isError());
    EXPECT_FALSE(moved.isSuccess());

    auto stored = moved.getError();
    EXPECT_EQ(*stored, testError);
}

TEST(ResultTest, MoveErrorCaseFromRef) {
    Result<Ref<RefClass>, int> result(std::make_unique<int>(testError));
    Result<Ref<RefClass>, int> moved(std::move(result));

    EXPECT_TRUE(moved.isError());
    EXPECT_FALSE(moved.isSuccess());

    auto stored = moved.getError();
    EXPECT_EQ(*stored, testError);
}

TEST(ResultTest, MoveErrorCaseFromVector) {
    Result<std::vector<float>, int> result(std::make_unique<int>(testError));
    Result<std::vector<float>, int> moved(std::move(result));

    EXPECT_TRUE(moved.isError());
    EXPECT_FALSE(moved.isSuccess());

    auto stored = moved.getError();
    EXPECT_EQ(*stored, testError);
}

TEST(ResultTest, ReturnErrorCaseFromVoid) {
    Result<void, int> result = []() { return std::make_unique<int>(testError); }();

    EXPECT_TRUE(result.isError());
    EXPECT_FALSE(result.isSuccess());

    auto stored = result.getError();
    EXPECT_EQ(*stored, testError);
}

TEST(ResultTest, ReturnErrorCaseFromPointer) {
    Result<float*, int> result = []() -> Result<float*, int> {
        return {std::make_unique<int>(testError)};
    }();

    EXPECT_TRUE(result.isError());
    EXPECT_FALSE(result.isSuccess());

    auto stored = result.getError();
    EXPECT_EQ(*stored, testError);
}

TEST(ResultTest, ReturnErrorCaseFromRef) {
    Result<Ref<RefClass>, int> result = []() -> Result<Ref<RefClass>, int> {
        return {std::make_unique<int>(testError)};
    }();

    EXPECT_TRUE(result.isError());
    EXPECT_FALSE(result.isSuccess());

    auto stored = result.getError();
    EXPECT_EQ(*stored, testError);
}

TEST(ResultTest, ReturnErrorCaseFromVector) {
    Result<std::vector<float>, int> result = []() -> Result<std::vector<float>, int> {
        return std::make_unique<int>(testError);
    }();

    EXPECT_TRUE(result.isError());
    EXPECT_FALSE(result.isSuccess());

    auto stored = result.getError();
    EXPECT_EQ(*stored, testError);
}

TEST(ResultTest, CreateSuccessCaseFromVoid) {
    Result<void, int> result;
    EXPECT_TRUE(result.isSuccess());
    EXPECT_FALSE(result.isError());
}

TEST(ResultTest, CreateSuccessCaseFromPointer) {
    Result<float*, int> result(&testSuccess);
    EXPECT_FALSE(result.isError());
    EXPECT_TRUE(result.isSuccess());

    auto stored = result.getSuccess();
    EXPECT_EQ(*stored, testSuccess);

    // Once the success has been delivered, result has an empty payload and no success/error state
    EXPECT_FALSE(result.isError());
    EXPECT_FALSE(result.isSuccess());
}

TEST(ResultTest, CreateSuccessCaseFromRef) {
    RefClass success;

    Ref<RefClass> refObject(&success);
    Result<Ref<RefClass>, int> result(std::move(refObject));

    EXPECT_FALSE(result.isError());
    EXPECT_TRUE(result.isSuccess());

    auto stored = result.getSuccess();
    EXPECT_EQ(stored.get(), &success);

    EXPECT_FALSE(result.isError());
    EXPECT_FALSE(result.isSuccess());
}

TEST(ResultTest, CreateSuccessCaseFromVector) {
    Result<std::vector<float>, int> result({1.0f});
    EXPECT_FALSE(result.isError());
    EXPECT_TRUE(result.isSuccess());

    auto stored = result.getSuccess();
    EXPECT_EQ(stored[0], 1.0f);

    EXPECT_FALSE(result.isError());
    EXPECT_FALSE(result.isSuccess());
}

TEST(ResultTest, MoveSuccessCaseFromVoid) {
    Result<void, int> result;
    Result<void, int> moved(std::move(result));
    EXPECT_TRUE(moved.isSuccess());
    EXPECT_FALSE(moved.isError());
}

TEST(ResultTest, MoveSuccessCaseFromPointer) {
    Result<float*, int> result(&testSuccess);
    Result<float*, int> moved(std::move(result));
    EXPECT_FALSE(moved.isError());
    EXPECT_TRUE(moved.isSuccess());

    auto stored = moved.getSuccess();
    EXPECT_EQ(*stored, testSuccess);

    EXPECT_FALSE(moved.isError());
    EXPECT_FALSE(moved.isSuccess());
}

TEST(ResultTest, MoveSuccessCaseFromRef) {
    RefClass success;

    Ref<RefClass> refObject(&success);
    Result<Ref<RefClass>, int> result(std::move(refObject));
    Result<Ref<RefClass>, int> moved(std::move(result));

    EXPECT_FALSE(moved.isError());
    EXPECT_TRUE(moved.isSuccess());

    auto stored = moved.getSuccess();
    EXPECT_EQ(stored.get(), &success);

    EXPECT_FALSE(moved.isError());
    EXPECT_FALSE(moved.isSuccess());
}

TEST(ResultTest, MoveSuccessCaseFromVector) {
    Result<std::vector<float>, int> result({1.0f});
    Result<std::vector<float>, int> moved(std::move(result));
    EXPECT_FALSE(moved.isError());
    EXPECT_TRUE(moved.isSuccess());

    auto stored = moved.getSuccess();
    EXPECT_EQ(stored[0], 1.0f);

    EXPECT_FALSE(moved.isError());
    EXPECT_FALSE(moved.isSuccess());
}

TEST(ResultTest, ReturnSuccessCaseFromVoid) {
    Result<void, int> result = []() -> Result<void, int> { return {}; }();
    EXPECT_TRUE(result.isSuccess());
    EXPECT_FALSE(result.isError());
}

TEST(ResultTest, ReturnSuccessCaseFromPointer) {
    Result<float*, int*> result = []() -> Result<float*, int*> { return {&testSuccess}; }();
    EXPECT_FALSE(result.isError());
    EXPECT_TRUE(result.isSuccess());

    auto stored = result.getSuccess();
    EXPECT_EQ(*stored, testSuccess);

    EXPECT_FALSE(result.isError());
    EXPECT_FALSE(result.isSuccess());
}

TEST(ResultTest, ReturnSuccessCaseFromRef) {
    RefClass success;
    Result<Ref<RefClass>, int> result = [&success]() -> Result<Ref<RefClass>, int> {
        return Ref<RefClass>(&success);
    }();

    EXPECT_FALSE(result.isError());
    EXPECT_TRUE(result.isSuccess());

    auto stored = result.getSuccess();
    EXPECT_EQ(stored.get(), &success);

    EXPECT_FALSE(result.isError());
    EXPECT_FALSE(result.isSuccess());
}

TEST(ResultTest, ReturnSuccessCaseFromVector) {
    Result<std::vector<float>, int> result = []() -> Result<std::vector<float>, int> {
        return {{1.0f}};
    }();

    EXPECT_FALSE(result.isError());
    EXPECT_TRUE(result.isSuccess());

    auto stored = result.getSuccess();
    EXPECT_EQ(stored[0], 1.0f);

    EXPECT_FALSE(result.isError());
    EXPECT_FALSE(result.isSuccess());
}
