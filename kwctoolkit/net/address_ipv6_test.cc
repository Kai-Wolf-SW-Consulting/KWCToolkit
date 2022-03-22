// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/net/address_ipv6.h"

#include <gtest/gtest.h>

using kwc::net::AddressIPv6;

TEST(AddressIPv6Test, CreateAddress) {
    auto addr = AddressIPv6::fromString("2001:cdba::3257:9652");
    ASSERT_TRUE(addr.isSuccess());
    ASSERT_TRUE(addr.getSuccess().toString() == "2001:cdba::3257:9652");
}

TEST(AddressIPv6Test, AssignAddress) {
    auto addr1 = AddressIPv6::fromString("2001:cdba::3257:9652");
    AddressIPv6 addr2 = addr1.getSuccess();
    ASSERT_TRUE(addr2.toString() == "2001:cdba::3257:9652");
}

TEST(AddressIPv6Test, CompareAddress) {
    auto addr = AddressIPv6::fromString("2001:cdba::3257:9652");
    ASSERT_TRUE(addr.getSuccess() == AddressIPv6::fromString("2001:cdba::3257:9652").getSuccess());
    ASSERT_TRUE(addr.getSuccess() != AddressIPv6::fromString("2001:cdba::3257:9625").getSuccess());
}

TEST(AddressIPv6Test, AddressNotation) {
    // any four-digit group of zeroes within an IPv6 address may be reduced to
    // a single zero or altogether omitted. Therefore, the following IPv6
    // addresses are similar and equally valid
    auto addr = AddressIPv6::fromString("2001:cdba:0000:0000:0000:0000:3257:9652");

    ASSERT_TRUE(addr.getSuccess() ==
                AddressIPv6::fromString("2001:cdba:0000:0000:0000:0000:3257:9652").getSuccess());
    ASSERT_TRUE(addr.getSuccess() ==
                AddressIPv6::fromString("2001:cdba:0:0:0:0:3257:9652").getSuccess());
    ASSERT_TRUE(addr.getSuccess() == AddressIPv6::fromString("2001:cdba::3257:9652").getSuccess());
}
