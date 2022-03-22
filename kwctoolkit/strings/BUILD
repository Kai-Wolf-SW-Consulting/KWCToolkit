# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "strings",
    srcs = [
        "string_split.cc",
        "string_utils.cc",
        "utf_string_conversion.cc"
    ],
    hdrs = [
        "string_split.h",
        "string_switch.h",
        "string_utils.h",
        "utf_string_conversion.h",
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
        "string_split_test.cc",
        "string_switch_test.cc",
    ],
    deps = [
        ":strings",
        "@com_google_googletest//:gtest_main",
    ],
)
