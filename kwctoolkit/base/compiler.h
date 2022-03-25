// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_COMPILER_H_
#define KWCTOOLKIT_BASE_COMPILER_H_

// Compiler detection
#if defined(__GNUC__)
    #define KWC_COMPILER_GCC 1
#elif defined(__clang__)
    #define KWC_COMPILER_CLANG 1
#elif defined(_MSC_VER)
    #define KWC_COMPILER_MSVC 1
#elif defined(__INTEL_COMPILER)
    #define KWC_COMPILER_INTEL 1
#else
    #error Please add support for your compiler in Compiler.h
#endif

// Processor architecture detection. For more info, see:
// http://www.agner.org/optimize/calling_conventions.pdf
#if defined(_M_X64) || defined(__x86_64__)
    #define KWC_ARCH_CPU_X86_FAMILY 1
    #define KWC_ARCH_CPU_X86_64 1
    #define KWC_ARCH_CPU_64_BITS 1
    #define KWC_ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(_M_IX86) || defined(__i386__)
    #define KWC_ARCH_CPU_X86_FAMILY 1
    #define KWC_ARCH_CPU_X86 1
    #define KWC_ARCH_CPU_32_BITS 1
    #define KWC_ARCH_CPU_LITTLE_ENDIAN 1
#elif (defined(__PPC64__) || defined(__PPC__)) && defined(__BIG_ENDIAN__)
    #define KWC_ARCH_CPU_PPC64_FAMILY 1
    #define KWC_ARCH_CPU_PPC64 1
    #define KWC_ARCH_CPU_64_BITS 1
    #define KWC_ARCH_CPU_BIG_ENDIAN 1
#elif defined(__PPC64__)
    #define KWC_ARCH_CPU_PPC64_FAMILY 1
    #define KWC_ARCH_CPU_PPC64 1
    #define KWC_ARCH_CPU_64_BITS 1
    #define KWC_ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__ARMEL__)
    #define KWC_ARCH_CPU_ARM_FAMILY 1
    #define KWC_ARCH_CPU_ARMEL 1
    #define KWC_ARCH_CPU_32_BITS 1
    #define KWC_ARCH_CPU_LITTLE_ENDIAN 1
#elif defined(__aarch64__)
    #define KWC_ARCH_CPU_ARM_FAMILY 1
    #define KWC_ARCH_CPU_ARM64 1
    #define KWC_ARCH_CPU_64_BITS 1
#else
    #error Please add support for your architecture in Compiler.h
#endif

// Following are some educated guesses of the L1 cache size for alignment
// purposes. This will allow specifying certain objects to be aligned on
// a cache line boundary.
// Please note that these macros should be replaced with the following
// once C++17 is generally available:
//
//    - std::hardware_constructive_interference_size
//    - std::hardware_destructive_interference_size
//
// See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0154r1.html
// for more information on this.
//
// For unknown architectures, a reasonable default estimation of 64 bytes
// should be sufficient, as larger values will waste more space whereas a
// smaller value will waste more time.
//
// The KWC_CACHELINE_ALIGNED macro models the behaviour of alignas()
// for supported compilers. The result of the application of this macro is
// always implementation-defined. Hence, use this with care
#if defined(KWC_COMPILER_GCC)
    #if defined(__i386__) || defined(__x86_64__)
        #define KWC_CACHELINE_SIZE 64
    #elif defined(__powerpc64__)
        #define KWC_CACHELINE_SIZE 128
    #elif defined(__aarch64__)
        #define KWC_CACHELINE_SIZE 64
    #elif defined(__arm__)
        #if defined(__ARM_ARCH_7A__)
            #define KWC_CACHELINE_SIZE 64
        #endif
    #endif
    #ifndef KWC_CACHELINE_SIZE
        #define KWC_CACHELINE_SIZE 64
    #endif
    #define KWC_CACHELINE_ALIGNED __attribute__((aligned(KWC_CACHELINE_SIZE)))
#elif defined(KWC_COMPILER_MSVC)
    #define KWC_CACHELINE_SIZE 64
    #define KWC_CACHELINE_ALIGNED __declspec(align(KWC_CACHELINE_SIZE))
#else
    #define KWC_CACHELINE_SIZE 64
    #define KWC_CACHELINE_ALIGNED
#endif

// Verify an expression during compile-time
#define KWC_COMPILE_ASSERT(expr, msg) static_assert(expr, #msg)

#if defined(KWC_COMPILER_GCC) || defined(KWC_COMPILER_CLANG)
    #define KWC_BUILTIN_UNREACHABLE() __builtin_unreachable()
#elif defined(KWC_COMPILER_MSVC)
    #define KWC_BUILTIN_UNREACHABLE() __assume(false)
#endif

// Define pretty function printing for various compilers
#if defined(KWC_COMPILER_GCC)
    #define KWC_PRETTY_FUNCTION __PRETTY_FUNCTION__
#elif defined(KWC_COMPILER_MSVC)
    #define KWC_PRETTY_FUNCTION __FUNCSIG__
#else
    // See https://en.cppreference.com/w/c/language/function_definition#func
    #define KWC_PRETTY_FUNCTION __func__
#endif

// Give branch indication hints to the (GCC) compiler: whether the if
// condition is likely to be true or false
#if defined(KWC_COMPILER_GCC) && __GNUC__ >= 4
    #define KWC_LIKELY(x) (__builtin_expect((x), 1))
    #define KWC_UNLIKELY(x) (__builtin_expect((x), 0))
#else
    #define KWC_LIKELYLIKELY(x) (x)
    #define KWC_UNLIKELY(x) (x)
#endif

// Document if a (shared) variable/field is protected by a lock.
// See https://clang.llvm.org/docs/ThreadSafetyAnalysis.html for documentation
#if defined(KWC_COMPILER_CLANG)
    #define KWC_GUARDED_BY(x) __attribute__((guarded_by(x)))
#else
    #define KWC_GUARDED_BY(x)
#endif

// Ensure that restrict is available
#if defined(KWC_COMPILER_MSVC)
    #define KWC_RESTRICT __restrict
#else
    #define KWC_RESTRICT __restrict__
#endif

// Add support for nodiscard if available
#if defined(KWC_COMPILER_GCC) || defined(KWC_COMPILER_CLANG)
    #if !defined(__has_cpp_attribute)
        #define __has_cpp_attribute(name) 0
    #endif

    // Use either warn_unused_result on clang or a c++1z extension for C++14.
    // Also avoid warn_unused_result with GCC as it is only a function attribute
    // as opossed to a type attribute
    #if __has_cpp_attribute(warn_unused_result) && defined(KWC_COMPILER_CLANG)
        #define KWC_NO_DISCARD __attribute__((warn_unused_result))
    #elif __cplusplus >= 201703L && __has_cpp_attribute(nodiscard)
        #define KWC_NO_DISCARD [[nodiscard]]
    #endif
#elif defined(KWC_COMPILER_MSVC)
    #if __cplusplus >= 201703L && _MSC_VER >= 1911
        #define KWC_NO_DISCARD [[nodiscard]]
    #endif
#endif
#if !defined(KWC_NO_DISCARD)
    #define KWC_NO_DISCARD
#endif

// Apparently (void) EXPR works on all compilers to silence unused variables
#define KWC_UNUSED(EXPR) (void)EXPR

#if defined(KWC_COMPILER_MSVC)
    #define KWC_MAYBE_UNUSED
#else
    #define KWC_MAYBE_UNUSED __attribute__((unused))
#endif

#endif  // KWCTOOLKIT_BASE_COMPILER_H_
