# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "image",
    srcs = [
        "image.cc",
        "image_decoder.cc",
        "image_utils.cc",
        "metadata.cc",
        "png_decoder.cc",
    ],
    hdrs = [
        "image.h",
        "image_decoder.h",
        "image_utils.h",
        "metadata.h",
        "png_decoder.h",
    ],
    deps = [
        "//kwctoolkit/base",
        "//kwctoolkit/file",
        "@libpng",
    ],
)
