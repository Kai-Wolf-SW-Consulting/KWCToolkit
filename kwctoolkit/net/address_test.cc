// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/net/address.h"

#include <gtest/gtest.h>

using kwc::net::Address;
using kwc::net::AddressIPv4;
using kwc::net::AddressIPv6;

TEST(AddressTest, CreateAddressIPv4) {
    auto addr = Address::fromString("127.0.0.1");
    ASSERT_TRUE(addr.isSuccess());
    ASSERT_TRUE(addr.getSuccess().toString() == "127.0.0.1");
    ASSERT_TRUE(addr.getSuccess().isIPv4());
}

TEST(AddressTest, CreateAddressIPv6) {
    auto addr = Address::fromString("2001:cdba::3257:9652");
    ASSERT_TRUE(addr.isSuccess());
    ASSERT_TRUE(addr.getSuccess().toString() == "2001:cdba::3257:9652");
    ASSERT_TRUE(addr.getSuccess().isIPv6());
}

TEST(AddressTest, AssignAddressIPv4) {
    auto addr = Address::fromString("192.168.0.1");
    ASSERT_TRUE(addr.getSuccess().isIPv4());
}

TEST(AddressTest, AssignAddressIPv6) {
    auto addr = Address::fromString("2001:cdba::1492:2342");
    ASSERT_TRUE(addr.getSuccess().isIPv6());
}

TEST(AddressTest, CompareAddressIPv4) {
    auto addr1 = AddressIPv4::fromString("192.168.0.1");
    auto addr2 = AddressIPv4::fromString("192.168.0.1");
    ASSERT_TRUE(addr1.getSuccess() == addr2.getSuccess());

    ASSERT_TRUE(addr1.getSuccess() != AddressIPv4::fromString("192.168.1.1").getSuccess());
}

TEST(AddressTest, CompareAddressIPv6) {
    auto addr1 = AddressIPv6::fromString("2001:cdba::1492:2342");
    auto addr2 = AddressIPv6::fromString("2001:cdba::1552:2342");
    ASSERT_TRUE(addr1.getSuccess() != addr2.getSuccess());

    ASSERT_TRUE(addr2.getSuccess() == AddressIPv6::fromString("2001:cdba::1552:2342").getSuccess());
}

TEST(AddressTest, IsValidIPv4Address) {
    ASSERT_TRUE(Address::isValid("192.168.17.23"));
    ASSERT_FALSE(Address::isValid("192.168.17.23.42"));
}

TEST(AddressTest, IsValidIPv6Address) {
    ASSERT_TRUE(Address::isValid("2001:cdba::1492:2342"));
    ASSERT_FALSE(Address::isValid("2001:cdba::1492:2342::::1492"));
}
