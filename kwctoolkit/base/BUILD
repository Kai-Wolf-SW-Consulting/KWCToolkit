# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "base",
    srcs = [
        "assert.cc",
        "check.cc",
        "cmdline_flags.cc",
        "error_trace.cc",
        "logging.cc",
        "ref_count.cc",
        "result.cc",
        "status.cc",
    ],
    hdrs = [
        "array_copy.h",
        "array_size.h",
        "assert.h",
        "callback.h",
        "callback_impl.h",
        "callback_types.h",
        "check.h",
        "cmdline_flags.h",
        "compiler.h",
        "error.h",
        "error_trace.h",
        "for_each_argument.h",
        "integral_types.h",
        "logging.h",
        "macros.h",
        "platform.h",
        "ref_count.h",
        "result.h",
        "rw_protected.h",
        "scope_guard.h",
        "status.h",
        "utils.h",
        ":build_flags_internal_h",
        "windows.h",
    ],
)

genrule(
    name = "build_flags_internal_h",
    srcs = [
        "build_flags_internal.h.in",
    ],
    outs = [
        "build_flags_internal.h",
    ],
    cmd = "awk '{ gsub(/\\$${ENABLE_ASSERTS}/, 0); print; }' $(<) > $(@)",
)

cc_test(
    name = "base_test",
    size = "small",
    srcs = [
        "array_copy_test.cc",
        "array_size_test.cc",
        "callback_test.cc",
        "cmdline_flags_test.cc",
        "ref_count_test.cc",
        "result_test.cc",
        "scope_guard_test.cc",
        "utils_test.cc",
    ],
    deps = [
        ":base",
        "@com_google_googletest//:gtest_main",
    ],
)
