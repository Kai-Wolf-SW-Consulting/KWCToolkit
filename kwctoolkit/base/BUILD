# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

package(default_visibility = ["//visibility:public"])

cc_library(
    name = "base",
    srcs = [
        "Assert.cc",
        "Check.cc",
        "CmdlineFlags.cc",
        "ErrorTrace.cc",
        "Logging.cc",
        "RefCount.cc",
        "Result.cc",
        "Status.cc",
    ],
    hdrs = [
        "ArrayCopy.h",
        "ArraySize.h",
        "Assert.h",
        "Callback.h",
        "CallbackImpl.h",
        "CallbackTypes.h",
        "Check.h",
        "CmdlineFlags.h",
        "Compiler.h",
        "Error.h",
        "ErrorTrace.h",
        "ForEachArgument.h",
        "IntegralTypes.h",
        "Logging.h",
        "Macros.h",
        "Platform.h",
        "RefCount.h",
        "Result.h",
        "RwProtected.h",
        "ScopeGuard.h",
        "Status.h",
        "Utils.h",
        ":BuildFlagsInternal_h",
    ],
)

genrule(
    name = "BuildFlagsInternal_h",
    srcs = [
        "BuildFlagsInternal.h.in",
    ],
    outs = [
        "BuildFlagsInternal.h",
    ],
    cmd = "awk '{ gsub(/\\$${ENABLE_ASSERTS}/, 0); print; }' $(<) > $(@)",
)
