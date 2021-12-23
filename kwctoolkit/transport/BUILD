# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "transport",
    srcs = [
        "HttpRequest.cc",
        "HttpResponse.cc",
        "HttpTransaction.cc",
        "SimpleHttpTransaction.cc",
    ],
    hdrs = [
        "HttpRequest.h",
        "HttpResponse.h",
        "HttpTransaction.h",
        "HttpTypes.h",
        "SimpleHttpTransaction.h",
    ],
    deps = [
        "//kwctoolkit/base",
        "//kwctoolkit/serialization",
        "//kwctoolkit/strings",
        "//kwctoolkit/system",
    ]
)

cc_test(
    name = "transport_test",
    size = "small",
    srcs = [
        "HttpTransactionTest.cc",
    ],
    deps = [
        ":transport",
        "@com_google_googletest//:gtest_main",
    ],
)
