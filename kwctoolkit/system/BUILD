# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

config_setting(
    name = "windows",
    constraint_values = ["@bazel_tools//platforms:windows"],
)

config_setting(
    name = "mac",
    constraint_values = ["@bazel_tools//platforms:osx"],
)

config_setting(
    name = "linux",
    constraint_values = ["@bazel_tools//platforms:linux"],
)

config_setting(
    name = "x86_64",
    constraint_values = ["@platforms//cpu:x86_64"],
)

objc_library(
    name = "system_mac",
    srcs = [
        "system_info_mac.mm",
        "system_memory_info_mac.mm",
    ],
    hdrs = [
        "system_info.h",
        "system_memory_info.h",
    ],
    deps = [
        "//kwctoolkit/base",
    ],
)

cc_library(
    name = "system",
    srcs = [
        "aligned_alloc.cc",
        "cmdline.cc",
        "environment.cc",
        "executor.cc",
        "feature_list.cc",
        "sleep.cc",
        "system_info.cc",
        "thread.cc",
        "thread_types.cc",
    ] + select({
        ":x86_64": [
            "cpu.cc",
        ],
        ":windows": [
            "system_info_win.cc",
            "system_memory_info_Win.cc",
            "timer_win.cc",
        ],
        ":mac": [
            "system_info_posix.cc",
            "timer_mac.cc",
        ],
        ":linux": [
            "process_internal_linux.cc",
            "system_info_linux.cc",
            "system_info_posix.cc",
            "system_memory_info_linux.cc",
            "timer_linux.cc",
        ],
    }),
    hdrs = [
        "aligned_alloc.h",
        "cmdline.h",
        "environment.h",
        "executor.h",
        "feature_list.h",
        "sleep.h",
        "system_info.h",
        "system_memory_info.h",
        "thread.h",
        "thread_helper.h",
        "thread_types.h",
        "timer.h",
    ] + select({
        ":x86_64": [
            "cpu.h",
        ],
        ":linux": [
            "process_internal_linux.h",
        ],
        "//conditions:default": [],
    }),
    copts = select({
        ":windows": [],
        "//conditions:default": ["-pthread"],
    }),
    linkopts = select({
        ":windows": [],
        "//conditions:default": ["-pthread"],
    }),
    deps = [
        "//kwctoolkit/base",
        "//kwctoolkit/file",
    ] + select({
        ":mac": [":system_mac"],
        "//conditions:default": [],
    }),
    linkstatic = True,
)

cc_test(
    name = "system_test",
    size = "small",
    srcs = [
        "aligned_alloc_test.cc",
        "cmdline_test.cc",
        "environment_test.cc",
        "system_info_test.cc",
        "thread_test.cc",
    ],
    deps = [
        ":system",
        "@com_google_googletest//:gtest_main",
    ] + select({
        ":mac": [":system_mac"],
        "//conditions:default": [],
    }),
)
