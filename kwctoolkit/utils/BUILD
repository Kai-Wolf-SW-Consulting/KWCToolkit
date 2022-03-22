# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "utils",
    srcs = [
        "base64.cc",
        "color_print.cc",
        "regex.cc",
    ],
    hdrs = [
        "base64.h",
        "benchmark.h",
        "color_print.h",
        "levenshtein.h",
        "regex.h",
        "zip.h",
    ],
    deps = [
        "//kwctoolkit/base",
        "//kwctoolkit/strings",
    ],
    linkstatic = True,
)

cc_test(
    name = "utils_test",
    size = "small",
    srcs = [
        "base64_test.cc",
        "levenshtein_test.cc",
        "regex_test.cc",
        "zip_test.cc",
    ],
    deps = [
        ":utils",
        "@com_google_googletest//:gtest_main",
    ],
)
