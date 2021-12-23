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

objc_library(
    name = "system_mac",
    srcs = [
        "SystemInfoMac.mm",
        "SystemMemoryInfoMac.mm",
    ],
    hdrs = [
        "SystemInfo.h",
        "SystemMemoryInfo.h",
    ],
    deps = [
        "//kwctoolkit/base",
    ],
)

cc_library(
    name = "system",
    srcs = [
        "AlignedAlloc.cc",
        "Cmdline.cc",
        "Cpu.cc",
        "Environment.cc",
        "Executor.cc",
        "FeatureList.cc",
        "Sleep.cc",
        "SystemInfo.cc",
        "Thread.cc",
        "ThreadTypes.cc",
    ] + select({
        ":windows": [
            "SystemInfoWin.cc",
            "SystemMemoryInfoWin.cc",
            "TimerWin.cc",
        ],
        ":mac": [
            "SystemInfoPosix.cc",
            "TimerMac.cc",
        ],
        ":linux": [
            "ProcessInternalLinux.cc",
            "SystemInfoLinux.cc",
            "SystemInfoPosix.cc",
            "SystemMemoryInfoLinux.cc",
            "TimerLinux.cc",
        ],
    }),
    hdrs = [
        "AlignedAlloc.h",
        "Cmdline.h",
        "Cpu.h",
        "Environment.h",
        "Executor.h",
        "FeatureList.h",
        "Sleep.h",
        "SystemInfo.h",
        "SystemMemoryInfo.h",
        "Thread.h",
        "ThreadHelper.h",
        "ThreadTypes.h",
        "Timer.h",
    ] + select({
        ":linux": [
            "ProcessInternalLinux.h",
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
        "AlignedAllocTest.cc",
        "CmdlineTest.cc",
        "EnvironmentTest.cc",
        "SystemInfoTest.cc",
        "ThreadTest.cc",
    ],
    deps = [
        ":system",
        "@com_google_googletest//:gtest_main",
    ] + select({
        ":mac": [":system_mac"],
        "//conditions:default": [],
    }),
)
