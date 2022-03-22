# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "audio",
    srcs = [
        "pcm_utils.cc",
        "sine_generator.cc",
    ],
    hdrs = [
        "dft.h",
        "pcm_utils.h",
        "sine_generator.h",
    ],
    deps = [
        "//kwctoolkit/base",
    ]
)
