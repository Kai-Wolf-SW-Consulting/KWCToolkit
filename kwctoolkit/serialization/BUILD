# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "serialization",
    srcs = [
        "data_reader.cc",
        "data_writer.cc",
        "in_memory_data_reader.cc",
        "istream_data_reader.cc",
        "string_data_writer.cc",
    ],
    hdrs = [
        "data_reader.h",
        "data_writer.h",
    ],
    deps = [
        "//kwctoolkit/base",
    ]
)

cc_test(
    name = "serialization_test",
    size = "small",
    srcs = [
        "data_reader_test.cc",
        "data_writer_test.cc",
    ],
    deps = [
        ":serialization",
        "@com_google_googletest//:gtest_main",
    ],
)
