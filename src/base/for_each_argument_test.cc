// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "base/for_each_argument.h"

#include <gtest/gtest.h>

#include <sstream>

namespace {
void PrintFloat(float f) {
    std::cout << f << ",";
}
}  // namespace

TEST(ForEachArgumentTest, ExecuteFunForEachArgument) {
    // Redirect standard output
    std::stringstream buffer;
    std::streambuf* sbuf = std::cout.rdbuf();
    std::cout.rdbuf(buffer.rdbuf());

    kwc::ForEachArgumentInOrder(PrintFloat, 1.2f, 2.3f, 3.4f, 4.5f);

    // Cleanup
    std::cout.rdbuf(sbuf);

    ASSERT_EQ(buffer.str(), "1.2,2.3,3.4,4.5,");
}