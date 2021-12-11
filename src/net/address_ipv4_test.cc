// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "net/address_ipv4.h"

#include <gtest/gtest.h>

using kwc::net::AddressIPv4;

TEST(AddressIPv4Test, CreateAddress) {
    AddressIPv4 addr = AddressIPv4::fromString("127.0.0.1");
    ASSERT_TRUE(addr.toString() == "127.0.0.1");
}

TEST(AddressIPv4Test, CreateDefaultAddress) {
    AddressIPv4 addr;
    ASSERT_TRUE(addr.toString() == "0.0.0.0");
}

TEST(AddressIPv4Test, AssignAddress) {
    AddressIPv4 addr1 = AddressIPv4::fromString("127.0.0.1");
    AddressIPv4 addr2 = addr1;
    ASSERT_TRUE(addr2.toString() == "127.0.0.1");
}

TEST(AddressIPv4Test, CompareAddress) {
    AddressIPv4 addr = AddressIPv4::fromString("192.168.0.1");
    ASSERT_TRUE(addr == AddressIPv4::fromString("192.168.0.1"));

    ASSERT_TRUE(addr != AddressIPv4::fromString("192.168.1.1"));
}

TEST(AddressIPv4Test, ExceptAddress) {
    ASSERT_ANY_THROW(AddressIPv4::fromString("128.0.0.XXX"));
}
