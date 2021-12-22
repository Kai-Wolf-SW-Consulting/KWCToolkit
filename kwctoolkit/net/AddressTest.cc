// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/net/Address.h"

#include <gtest/gtest.h>

using kwc::net::Address;
using kwc::net::AddressIPv4;
using kwc::net::AddressIPv6;

TEST(AddressTest, CreateAddressIPv4) {
    Address addr = Address::fromString("127.0.0.1");
    ASSERT_TRUE(addr.toString() == "127.0.0.1");
    ASSERT_TRUE(addr.isIPv4());
}

TEST(AddressTest, CreateAddressIPv6) {
    Address addr = Address::fromString("2001:cdba::3257:9652");
    ASSERT_TRUE(addr.toString() == "2001:cdba::3257:9652");
    ASSERT_TRUE(addr.isIPv6());
}

TEST(AddressTest, AssignAddressIPv4) {
    Address addr = Address::fromString("192.168.0.1");
    ASSERT_TRUE(addr.isIPv4());
}

TEST(AddressTest, AssignAddressIPv6) {
    Address addr = Address::fromString("2001:cdba::1492:2342");
    ASSERT_TRUE(addr.isIPv6());
}

TEST(AddressTest, CompareAddressIPv4) {
    Address addr1 = AddressIPv4::fromString("192.168.0.1");
    Address addr2 = AddressIPv4::fromString("192.168.0.1");
    ASSERT_TRUE(addr1 == addr2);

    ASSERT_TRUE(addr1 != AddressIPv4::fromString("192.168.1.1"));
}

TEST(AddressTest, CompareAddressIPv6) {
    Address addr1 = AddressIPv6::fromString("2001:cdba::1492:2342");
    Address addr2 = AddressIPv6::fromString("2001:cdba::1552:2342");
    ASSERT_TRUE(addr1 != addr2);

    ASSERT_TRUE(addr2 == AddressIPv6::fromString("2001:cdba::1552:2342"));
}

TEST(AddressTest, IsValidIPv4Address) {
    ASSERT_TRUE(Address::isValid("192.168.17.23"));
    ASSERT_FALSE(Address::isValid("192.168.17.23.42"));
}

TEST(AddressTest, IsValidIPv6Address) {
    ASSERT_TRUE(Address::isValid("2001:cdba::1492:2342"));
    ASSERT_FALSE(Address::isValid("2001:cdba::1492:2342::::1492"));
}
