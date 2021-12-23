# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "strings",
    srcs = [
        "StringSplit.cc",
        "StringUtils.cc",
        "UtfStringConversion.cc"
    ],
    hdrs = [
        "StringSplit.h",
        "StringSwitch.h",
        "StringUtils.h",
        "UtfStringConversion.h",
    ],
    deps = [
        "//kwctoolkit/base",
        "//third_party/icu/v60.1:icu",
    ]
)

cc_test(
    name = "strings_test",
    size = "small",
    srcs = [
        "StringSplitTest.cc",
        "StringSwitchTest.cc",
    ],
    deps = [
        ":strings",
        "@com_google_googletest//:gtest_main",
    ],
)
