// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/base/Result.h"

#include <gtest/gtest.h>

#include <memory>
#include <vector>

#include "kwctoolkit/base/IntegralTypes.h"
#include "kwctoolkit/base/RefCount.h"

using namespace kwc;
using namespace kwc::base;

namespace {
int test_error = 17;
float test_success = 23.0f;

class RefClass : public RefCount {
  public:
    int foo_ = 0;
};
}  // namespace

TEST(ResultTest, CreateErrorCaseFromVoid) {
    Result<void, int> result(std::make_unique<int>(test_error));
    EXPECT_TRUE(result.isError());
    EXPECT_FALSE(result.isSuccess());

    auto stored = result.getError();
    EXPECT_EQ(*stored, test_error);
}

TEST(ResultTest, CreateErrorCaseFromPointer) {
    Result<float*, int> result(std::make_unique<int>(test_error));
    EXPECT_TRUE(result.isError());
    EXPECT_FALSE(result.isSuccess());

    auto stored = result.getError();
    EXPECT_EQ(*stored, test_error);
}

TEST(ResultTest, CreateErrorCaseFromRef) {
    Result<Ref<RefClass>, int> result(std::make_unique<int>(test_error));
    EXPECT_TRUE(result.isError());
    EXPECT_FALSE(result.isSuccess());

    auto stored = result.getError();
    EXPECT_EQ(*stored, test_error);
}

TEST(ResultTest, CreateErrorCaseFromVector) {
    Result<std::vector<float>, int> result(std::make_unique<int>(test_error));
    EXPECT_TRUE(result.isError());
    EXPECT_FALSE(result.isSuccess());

    auto stored = result.getError();
    EXPECT_EQ(*stored, test_error);
}

TEST(ResultTest, MoveErrorCaseFromVoid) {
    Result<void, int> result(std::make_unique<int>(test_error));
    Result<void, int> moved_result(std::move(result));

    EXPECT_TRUE(moved_result.isError());
    EXPECT_FALSE(moved_result.isSuccess());

    auto stored = moved_result.getError();
    EXPECT_EQ(*stored, test_error);
}

TEST(ResultTest, MoveErrorCaseFromPointer) {
    Result<float*, int> result(std::make_unique<int>(test_error));
    Result<float*, int> moved(std::move(result));

    EXPECT_TRUE(moved.isError());
    EXPECT_FALSE(moved.isSuccess());

    auto stored = moved.getError();
    EXPECT_EQ(*stored, test_error);
}

TEST(ResultTest, MoveErrorCaseFromRef) {
    Result<Ref<RefClass>, int> result(std::make_unique<int>(test_error));
    Result<Ref<RefClass>, int> moved(std::move(result));

    EXPECT_TRUE(moved.isError());
    EXPECT_FALSE(moved.isSuccess());

    auto stored = moved.getError();
    EXPECT_EQ(*stored, test_error);
}

TEST(ResultTest, MoveErrorCaseFromVector) {
    Result<std::vector<float>, int> result(std::make_unique<int>(test_error));
    Result<std::vector<float>, int> moved(std::move(result));

    EXPECT_TRUE(moved.isError());
    EXPECT_FALSE(moved.isSuccess());

    auto stored = moved.getError();
    EXPECT_EQ(*stored, test_error);
}

TEST(ResultTest, ReturnErrorCaseFromVoid) {
    Result<void, int> result = []() { return std::make_unique<int>(test_error); }();

    EXPECT_TRUE(result.isError());
    EXPECT_FALSE(result.isSuccess());

    auto stored = result.getError();
    EXPECT_EQ(*stored, test_error);
}

TEST(ResultTest, ReturnErrorCaseFromPointer) {
    Result<float*, int> result = []() -> Result<float*, int> {
        return {std::make_unique<int>(test_error)};
    }();

    EXPECT_TRUE(result.isError());
    EXPECT_FALSE(result.isSuccess());

    auto stored = result.getError();
    EXPECT_EQ(*stored, test_error);
}

TEST(ResultTest, ReturnErrorCaseFromRef) {
    Result<Ref<RefClass>, int> result = []() -> Result<Ref<RefClass>, int> {
        return {std::make_unique<int>(test_error)};
    }();

    EXPECT_TRUE(result.isError());
    EXPECT_FALSE(result.isSuccess());

    auto stored = result.getError();
    EXPECT_EQ(*stored, test_error);
}

TEST(ResultTest, ReturnErrorCaseFromVector) {
    Result<std::vector<float>, int> result = []() -> Result<std::vector<float>, int> {
        return std::make_unique<int>(test_error);
    }();

    EXPECT_TRUE(result.isError());
    EXPECT_FALSE(result.isSuccess());

    auto stored = result.getError();
    EXPECT_EQ(*stored, test_error);
}

TEST(ResultTest, CreateSuccessCaseFromVoid) {
    Result<void, int> result;
    EXPECT_TRUE(result.isSuccess());
    EXPECT_FALSE(result.isError());
}

TEST(ResultTest, CreateSuccessCaseFromPointer) {
    Result<float*, int> result(&test_success);
    EXPECT_FALSE(result.isError());
    EXPECT_TRUE(result.isSuccess());

    auto stored = result.getSuccess();
    EXPECT_EQ(*stored, test_success);

    // Once the success has been delivered, result has an empty payload and no success/error state_
    EXPECT_FALSE(result.isError());
    EXPECT_FALSE(result.isSuccess());
}

TEST(ResultTest, CreateSuccessCaseFromRef) {
    RefClass success;

    Ref<RefClass> ref_object(&success);
    Result<Ref<RefClass>, int> result(std::move(ref_object));

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
    Result<float*, int> result(&test_success);
    Result<float*, int> moved(std::move(result));
    EXPECT_FALSE(moved.isError());
    EXPECT_TRUE(moved.isSuccess());

    auto stored = moved.getSuccess();
    EXPECT_EQ(*stored, test_success);

    EXPECT_FALSE(moved.isError());
    EXPECT_FALSE(moved.isSuccess());
}

TEST(ResultTest, MoveSuccessCaseFromRef) {
    RefClass success;

    Ref<RefClass> ref_object(&success);
    Result<Ref<RefClass>, int> result(std::move(ref_object));
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
    Result<float*, int*> result = []() -> Result<float*, int*> { return {&test_success}; }();
    EXPECT_FALSE(result.isError());
    EXPECT_TRUE(result.isSuccess());

    auto stored = result.getSuccess();
    EXPECT_EQ(*stored, test_success);

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
