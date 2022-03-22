# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "transport",
    srcs = [
        "http_request.cc",
        "http_response.cc",
        "http_transaction.cc",
        "simple_http_transaction.cc",
    ],
    hdrs = [
        "http_request.h",
        "http_response.h",
        "http_transaction.h",
        "http_types.h",
        "simple_http_transaction.h",
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
        "http_transaction_test.cc",
    ],
    deps = [
        ":transport",
        "@com_google_googletest//:gtest_main",
    ],
)
