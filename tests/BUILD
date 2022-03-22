# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

cc_test(
    name = "kwc_integrationtests",
    size = "medium",
    srcs = [
        "integrationtests_main.cc",
        "integrationtests/decode_png_images_test.cc",
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

