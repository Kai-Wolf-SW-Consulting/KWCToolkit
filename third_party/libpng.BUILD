# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

config_setting(
    name = "windows",
    constraint_values = ["@bazel_tools//platforms:windows"],
)

config_setting(
    name = "arm64",
    constraint_values = ["@platforms//cpu:arm64"],
)

cc_library(
    name = "libpng",
    srcs = [
        "png.c",
        "pngdebug.h",
        "pngerror.c",
        "pngget.c",
        "pnginfo.h",
        "pnglibconf.h",
        "pngmem.c",
        "pngpread.c",
        "pngpriv.h",
        "pngread.c",
        "pngrio.c",
        "pngrtran.c",
        "pngrutil.c",
        "pngset.c",
        "pngstruct.h",
        "pngtrans.c",
        "pngwio.c",
        "pngwrite.c",
        "pngwtran.c",
        "pngwutil.c",
    ] + select({
        ":windows": [
            "intel/intel_init.c",
            "intel/filter_sse2_intrinsics.c",
        ],
        "//conditions:default": [
        ],
    }),
    hdrs = [
        "png.h",
        "pngconf.h",
    ],
    copts = select({
        ":windows": ["-DPNG_INTEL_SSE_OPT=1"],
        ":arm64": ["-DPNG_ARM_NEON=off"],
        "//conditions:default": [],
    }),
    includes = ["."],
    linkopts = select({
        ":windows": [],
        "//conditions:default": ["-lm"],
    }),
    visibility = ["//visibility:public"],
    deps = ["@zlib"],
)

genrule(
    name = "gen_pnglibconf_h",
    srcs = ["scripts/pnglibconf.h.prebuilt"],
    outs = ["pnglibconf.h"],
    cmd = "sed -e 's/PNG_ZLIB_VERNUM 0/PNG_ZLIB_VERNUM 0x12b0/' $< >$@",
)
