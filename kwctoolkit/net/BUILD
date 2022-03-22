# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "net",
    srcs = [
        "address_ipv4.cc",
        "address_ipv6.cc",
    ],
    hdrs = [
        "address.h",
        "address_ipv4.h",
        "address_ipv6.h",
        "address_types.h",
    ],
    deps = [
        "//kwctoolkit/base",
    ]
)

cc_test(
    name = "net_test",
    size = "small",
    srcs = [
        "address_ipv4_test.cc",
        "address_ipv6_test.cc",
        "address_test.cc",
    ],
    deps = [
        ":net",
        "@com_google_googletest//:gtest_main",
    ],
)
