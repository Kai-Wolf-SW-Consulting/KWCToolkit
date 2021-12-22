// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include <gtest/gtest.h>

#include <kwctoolkit/base/CheckedInteger.h>
#include <kwctoolkit/base/IntegralTypes.h>

using namespace kwc;

class CheckedIntegerTest : public testing::Test {
  protected:
    using Unsigned = base::CheckedInteger<struct UnsignedTag, uint32>;
    using Signed = base::CheckedInteger<struct SignedTag, int32>;
};

TEST_F(CheckedIntegerTest, CheckConstructionAndCasting) {
    Signed value(17);
    EXPECT_EQ(static_cast<int32>(value), 17);

    Unsigned uvalue(23);
    EXPECT_EQ(static_cast<uint32>(uvalue), 23);

    static_assert(static_cast<int32>(Signed(42)) == 42, "");
    static_assert(static_cast<uint32>(Unsigned(43)) == 43, "");
}

TEST_F(CheckedIntegerTest, CheckComparison) {
    Unsigned value(8);

    EXPECT_TRUE(value < Unsigned(9));
    EXPECT_TRUE(value <= Unsigned(9));
    EXPECT_TRUE(value == Unsigned(8));
    EXPECT_TRUE(value >= Unsigned(8));
    EXPECT_TRUE(value >= Unsigned(7));
    EXPECT_TRUE(value > Unsigned(7));
    EXPECT_TRUE(value != Unsigned(7));
}

TEST_F(CheckedIntegerTest, CheckArithmetic) {
    {
        // Prefix increment
        Signed value(0);
        EXPECT_EQ(++value, Signed(1));
        EXPECT_EQ(value, Signed(1));
    }

    {
        // Postfix increment
        Signed value(0);
        EXPECT_EQ(value++, Signed(0));
        EXPECT_EQ(value, Signed(1));
    }

    {
        // Prefix decrement
        Signed value(0);
        EXPECT_EQ(--value, Signed(-1));
        EXPECT_EQ(value, Signed(-1));
    }

    {
        // Prefix decrement
        Signed value(0);
        EXPECT_EQ(value--, Signed(0));
        EXPECT_EQ(value, Signed(-1));
    }

    {
        // Signed addition
        Signed a(3);
        Signed b(-4);
        Signed c = a + b;
        EXPECT_EQ(a, Signed(3));
        EXPECT_EQ(b, Signed(-4));
        EXPECT_EQ(c, Signed(-1));
    }

    {
        // Signed subtraction
        Signed a(3);
        Signed b(-4);
        Signed c = a - b;
        EXPECT_EQ(a, Signed(3));
        EXPECT_EQ(b, Signed(-4));
        EXPECT_EQ(c, Signed(7));
    }

    {
        // Unsigned addition
        Unsigned a(9);
        Unsigned b(3);
        Unsigned c = a + b;
        EXPECT_EQ(a, Unsigned(9));
        EXPECT_EQ(b, Unsigned(3));
        EXPECT_EQ(c, Unsigned(12));
    }

    {
        // Unsigned subtraction
        Unsigned a(9);
        Unsigned b(2);
        Unsigned c = a - b;
        EXPECT_EQ(a, Unsigned(9));
        EXPECT_EQ(b, Unsigned(2));
        EXPECT_EQ(c, Unsigned(7));
    }

    {
        // Negation
        Signed a(5);
        Signed b = -a;
        EXPECT_EQ(a, Signed(5));
        EXPECT_EQ(b, Signed(-5));
    }
}