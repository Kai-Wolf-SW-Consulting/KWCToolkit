# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "utils",
    srcs = [
        "Base64.cc",
        "ColorPrint.cc",
        "Regex.cc",
    ],
    hdrs = [
        "Base64.h",
        "Benchmark.h",
        "ColorPrint.h",
        "Levenshtein.h",
        "Regex.h",
        "Zip.h",
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
        "Base64Test.cc",
        "LevenshteinTest.cc",
        "RegexTest.cc",
        "ZipTest.cc",
    ],
    deps = [
        ":utils",
        "@com_google_googletest//:gtest_main",
    ],
)
