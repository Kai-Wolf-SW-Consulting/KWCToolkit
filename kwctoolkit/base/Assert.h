// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_ASSERT_H_
#define KWCTOOLKIT_BASE_ASSERT_H_

#include "kwctoolkit/base/BuildFlagsInternal.h"
#include "kwctoolkit/base/Compiler.h"

// These Assert should always be used in favor of the regular C stdlib
// style assert() function. In a debug build KWC_ASSERT (cond) will
// trigger an error, whereas on a release build it will do nothing (at runtime)
// These assert feature:
//   - Logging of the error file together with line and function information
//   - Use assert information to help the compiler for release builds.
//     See: https://clang.llvm.org/docs/LanguageExtensions.html for more info.

// Prevent MSVC warning trigger when compiling with /W4
#if defined(KWC_COMPILER_MSVC)
    #define KWC_ASSERT_LOOP_CONDITION (0, 0)
#else
    #define KWC_ASSERT_LOOP_CONDITION (0)
#endif

#if BUILD_FLAG(ENABLE_ASSERTS)
    #define KWC_ASSERT_HELPER(file, func, line, condition)         \
        do {                                                       \
            if (!(condition)) {                                    \
                handleAssertFailure(file, func, line, #condition); \
            }                                                      \
        } while (KWC_ASSERT_LOOP_CONDITION)
#else
    #if defined(KWC_COMPILER_MSVC)
        #define KWC_ASSERT_HELPER(file, func, line, condition) __assume(condition)
    #elif defined(KWC_COMPILER_CLANG) && defined(__builtin_assume)
        #define KWC_ASSERT_HELPER(file, func, line, condition) __builtin_assume(condition)
    #else
        #if defined(ABORT_ON_ASSERT)
            #define KWC_ASSERT_HELPER(file, func, line, condition) \
                do {                                               \
                    if (!(condition)) {                            \
                        ERROR(#condition);                         \
                        std::abort();                              \
                    }                                              \
                } while (KWC_ASSERT_LOOP_CONDITION)
        #else
            #define KWC_ASSERT_HELPER(file, func, line, condition) \
                do {                                               \
                    KWC_UNUSED((condition));                       \
                } while (KWC_ASSERT_LOOP_CONDITION)
        #endif
    #endif
#endif

#define KWC_ASSERT(condition) KWC_ASSERT_HELPER(__FILE__, __func__, __LINE__, condition)

#define KWC_FAIL(msg) \
    do {              \
        ERROR(msg);   \
        std::abort(); \
    } while (KWC_ASSERT_LOOP_CONDITION)

#define KWC_UNREACHABLE()                                                 \
    do {                                                                  \
        KWC_ASSERT(KWC_ASSERT_LOOP_CONDITION && "Unreachable code_ hit"); \
        KWC_BUILTIN_UNREACHABLE();                                        \
    } while (KWC_ASSERT_LOOP_CONDITION)

void HandleAssertFailure(const char* file, const char* fn, int line, const char* condition);

#endif  // KWCTOOLKIT_BASE_ASSERT_H_
