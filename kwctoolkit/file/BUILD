# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "file",
    srcs = [
        "File.cc",
        "FileEnumerator.cc",
        "FileInfo.cc",
        "FilePath.cc",
        "FileUtils.cc",
    ],
    hdrs = [
        "File.h",
        "FileEnumerator.h",
        "FileInfo.h",
        "FilePath.h",
        "FilePathConstants.h",
        "FileUtils.h",
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
        "FileEnumeratorTest.cc",
        "FilePathTest.cc",
        "FileTest.cc",
    ],
    deps = [
        ":file",
        "@com_google_googletest//:gtest_main",
    ],
)
