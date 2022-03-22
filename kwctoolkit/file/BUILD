# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "file",
    srcs = [
        "file.cc",
        "file_enumerator.cc",
        "file_info.cc",
        "file_path.cc",
        "file_utils.cc",
    ],
    hdrs = [
        "file.h",
        "file_enumerator.h",
        "file_info.h",
        "file_path.h",
        "file_path_constants.h",
        "file_utils.h",
    ],
    deps = [
        "//kwctoolkit/base",
        "//kwctoolkit/strings",
    ],
)

cc_test(
    name = "file_test",
    size = "small",
    srcs = [
        "file_enumerator_test.cc",
        "file_path_test.cc",
        "file_test.cc",
    ],
    deps = [
        ":file",
        "@com_google_googletest//:gtest_main",
    ],
)
