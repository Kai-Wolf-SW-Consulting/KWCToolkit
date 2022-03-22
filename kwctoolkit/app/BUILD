# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "app",
    srcs = [
        "app.cc",
    ],
    hdrs = [
        "app.h",
        ":app_info_h",
    ],
    deps = [
        "//kwctoolkit/base",
        "//kwctoolkit/file",
        "//kwctoolkit/strings",
        "//kwctoolkit/system",
    ],
)

genrule(
    name = "app_info_h",
    srcs = [
        "app_info.h.in",
    ],
    outs = [
        "app_info.h",
    ],
    cmd = "awk '{ gsub(/^#cmakedefine/, \"#define\"); \
        gsub(/@PROJECT_VERSION_MAJOR@/, 0); \
        gsub(/@PROJECT_VERSION_MINOR@/, 0); \
        gsub(/@PROJECT_VERSION_PATCH@/, 1); \
        gsub(/@APPLICATION_CODENAME@/, \"KWCToolkit\"); \
        print; }' $(<) > $(@)",
)

cc_binary(
    name = "kwc_main",
    srcs = [
        "main.cc"
    ],
    deps = [
        ":app"
    ],
)

cc_test(
    name = "app_test",
    size = "small",
    srcs = [
        "app_test.cc",
    ],
    deps = [
        ":app",
        "@com_google_googletest//:gtest_main",
    ],
)
