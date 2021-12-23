# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "image",
    srcs = [
        "Image.cc",
        "ImageDecoder.cc",
        "Metadata.cc",
        "PngDecoder.cc",
    ],
    hdrs = [
        "Image.h",
        "ImageDecoder.h",
        "Metadata.h",
        "PngDecoder.h",
    ],
    deps = [
        "//kwctoolkit/base",
        "//kwctoolkit/file",
        "@libpng",
    ],
)
