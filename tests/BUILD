# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

cc_test(
    name = "kwc_unittests",
    size = "small",
    srcs = [
        "unittests_main.cc",
    ],
    deps = [
        "//kwctoolkit/base",
        "//kwctoolkit/strings",
        "//kwctoolkit/file",
        "//kwctoolkit/net",
        "//kwctoolkit/serialization",
        "//kwctoolkit/system",
        "//kwctoolkit/transport",
        "//kwctoolkit/utils",
        "@com_google_googletest//:gtest",
    ]
)

genrule(
    name ="assets_h",
    srcs = [
        "assets.h.in",
    ],
    outs = [
        "assets.h",
    ],
    cmd = "awk '{ gsub(/@assets_path@/, \"tests/test_data\"); print; }' $(<) > $(@)",
)

cc_test(
    name = "kwc_integrationtests",
    size = "medium",
    srcs = [
        "integrationtests_main.cc",
        "integrationtests/decode_png_images_test.cc",
        ":assets_h"
    ],
    data = [
        "test_data/basket_rgb.png",
    ],
    deps = [
        "//kwctoolkit/base",
        "//kwctoolkit/file",
        "//kwctoolkit/image",
        "//kwctoolkit/strings",
        "@com_google_googletest//:gtest",
    ],
)

