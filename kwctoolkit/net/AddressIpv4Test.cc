// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/net/AddressIpv4.h"

#include <gtest/gtest.h>

using kwc::net::AddressIPv4;

TEST(AddressIPv4Test, CreateAddress) {
    auto addr = AddressIPv4::fromString("127.0.0.1");
    ASSERT_TRUE(addr.isSuccess());
    ASSERT_TRUE(addr.getSuccess().toString() == "127.0.0.1");
}

TEST(AddressIPv4Test, CreateDefaultAddress) {
    AddressIPv4 addr;
    ASSERT_TRUE(addr.toString() == "0.0.0.0");
}

TEST(AddressIPv4Test, AssignAddress) {
    auto addr1 = AddressIPv4::fromString("127.0.0.1");
    ASSERT_TRUE(addr1.isSuccess());
    auto addr2 = addr1.getSuccess();
    ASSERT_TRUE(addr2.toString() == "127.0.0.1");
}

TEST(AddressIPv4Test, CompareAddress) {
    auto addr = AddressIPv4::fromString("192.168.0.1");
    ASSERT_TRUE(addr.isSuccess());
    ASSERT_TRUE(addr.getSuccess() == AddressIPv4::fromString("192.168.0.1").getSuccess());

    ASSERT_TRUE(addr.getSuccess() != AddressIPv4::fromString("192.168.1.1").getSuccess());
}
