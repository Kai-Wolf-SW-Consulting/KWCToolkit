# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "net",
    srcs = [
        "AddressIpv4.cc",
        "AddressIpv6.cc",
    ],
    hdrs = [
        "Address.h",
        "AddressIpv6.h",
        "AddressIpv4.h",
        "AddressTypes.h",
    ],
    deps = [
        "//kwctoolkit/base",
    ]
)

cc_test(
    name = "net_test",
    size = "small",
    srcs = [
        "AddressIpv4Test.cc",
        "AddressIpv6Test.cc",
        "AddressTest.cc",
    ],
    deps = [
        ":net",
        "@com_google_googletest//:gtest_main",
    ],
)
