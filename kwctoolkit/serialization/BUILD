# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "serialization",
    srcs = [
        "DataReader.cc",
        "DataWriter.cc",
        "InMemoryDataReader.cc",
        "IstreamDataReader.cc",
        "StringDataWriter.cc",
    ],
    hdrs = [
        "DataReader.h",
        "DataWriter.h",
    ],
    deps = [
        "//kwctoolkit/base",
    ]
)

cc_test(
    name = "serialization_test",
    size = "small",
    srcs = [
        "DataReaderTest.cc",
        "DataWriterTest.cc",
    ],
    deps = [
        ":serialization",
        "@com_google_googletest//:gtest_main",
    ],
)
