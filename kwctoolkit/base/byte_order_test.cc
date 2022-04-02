// Copyright (c) 2022, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/base/byte_order.h"

#include <gtest/gtest.h>

#include <cstdint>

#include "kwctoolkit/base/integral_types.h"

using namespace kwc;

TEST(ByteOrderTest, TestMemSet) {
    uint8 buf[8] = {0u, 0u, 0u, 0u, 0u, 0u, 0u, 0u};
    base::Set8(buf, 0, 0xab);
    base::Set8(buf, 1, 0x12);
    EXPECT_EQ(0xab, buf[0]);
    EXPECT_EQ(0x12, buf[1]);
    base::SetBE16(buf, 0x1234);
    EXPECT_EQ(0x12, buf[0]);
    EXPECT_EQ(0x34, buf[1]);
    base::SetLE16(buf, 0x1234);
    EXPECT_EQ(0x34, buf[0]);
    EXPECT_EQ(0x12, buf[1]);
    base::SetBE32(buf, 0x12345678);
    EXPECT_EQ(0x12, buf[0]);
    EXPECT_EQ(0x34, buf[1]);
    EXPECT_EQ(0x56, buf[2]);
    EXPECT_EQ(0x78, buf[3]);
    base::SetLE32(buf, 0x12345678);
    EXPECT_EQ(0x78, buf[0]);
    EXPECT_EQ(0x56, buf[1]);
    EXPECT_EQ(0x34, buf[2]);
    EXPECT_EQ(0x12, buf[3]);
    base::SetBE64(buf, UINT64_C(0x0123456789abcdef));
    EXPECT_EQ(0x01, buf[0]);
    EXPECT_EQ(0x23, buf[1]);
    EXPECT_EQ(0x45, buf[2]);
    EXPECT_EQ(0x67, buf[3]);
    EXPECT_EQ(0x89, buf[4]);
    EXPECT_EQ(0xab, buf[5]);
    EXPECT_EQ(0xcd, buf[6]);
    EXPECT_EQ(0xef, buf[7]);
    base::SetLE64(buf, UINT64_C(0x0123456789abcdef));
    EXPECT_EQ(0xef, buf[0]);
    EXPECT_EQ(0xcd, buf[1]);
    EXPECT_EQ(0xab, buf[2]);
    EXPECT_EQ(0x89, buf[3]);
    EXPECT_EQ(0x67, buf[4]);
    EXPECT_EQ(0x45, buf[5]);
    EXPECT_EQ(0x23, buf[6]);
    EXPECT_EQ(0x01, buf[7]);
}

TEST(ByteOrderTest, TestMemGet) {
    uint8 buf[8];
    buf[0] = 0x01u;
    buf[1] = 0x23u;
    buf[2] = 0x45u;
    buf[3] = 0x67u;
    buf[4] = 0x89u;
    buf[5] = 0xabu;
    buf[6] = 0xcdu;
    buf[7] = 0xefu;
    EXPECT_EQ(0x01u, base::Get8(buf, 0));
    EXPECT_EQ(0x23u, base::Get8(buf, 1));
    EXPECT_EQ(0x0123u, base::GetBE16(buf));
    EXPECT_EQ(0x2301u, base::GetLE16(buf));
    EXPECT_EQ(0x01234567u, base::GetBE32(buf));
    EXPECT_EQ(0x67452301u, base::GetLE32(buf));
    EXPECT_EQ(UINT64_C(0x0123456789abcdef), base::GetBE64(buf));
    EXPECT_EQ(UINT64_C(0xefcdab8967452301), base::GetLE64(buf));
}