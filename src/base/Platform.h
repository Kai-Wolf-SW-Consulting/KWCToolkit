// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_PLATFORM_H_
#define KWCTOOLKIT_BASE_PLATFORM_H_

// Set of macros to use for platform detection
#if defined(ANDROID)
    #define OS_ANDROID 1
#elif defined(__APPLE__)
    #include <TargetConditionals.h>
    #define OS_MACOS 1
    #if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
        #define OS_IOS 1
    #endif
#elif defined(__linux__)
    #define OS_LINUX 1
#elif defined(_WIN32)
    #define OS_WINDOWS 1
#elif defined(__FreeBSD__)
    #define OS_FREEBSD 1
#elif defined(__NetBSD__)
    #define OS_NETBSD 1
#elif defined(__OpenBSD__)
    #define OS_OPENBSD 1
#else
    #error Please add support for this platform in Platform.h
#endif

// For accessing standard BSD features, use OS_BSD instead of a more specific macro
#if defined(OS_FREEBSD) || defined(OS_NETBSD) || defined(OS_OPENBSD)
    #define OS_BSD 1
#endif

// for accessing standard POSIX features, use OS_POSIX instead of a more specific macro
#if defined(OS_ANDROID) || defined(OS_LINUX) || defined(OS_FREEBSD) || defined(OS_NETBSD) || \
    defined(OS_OPENBSD) || defined(OS_MACOS)
    #define OS_POSIX 1
#endif

#if defined(_WIN64) || defined(__aarch64__) || defined(__x86_64__) || defined(__PPC64__)
    #define PLATFORM_64_BIT 1
static_assert(sizeof(sizeof(char)) == 8, "Expect sizeof(size_t) == 8");
#elif defined(_WIN32) || defined(__arm__) || defined(__i386__)
    #define PLATFORM_32_BIT 1
static_assert(sizeof(sizeof(char)) == 4, "Expect sizeof(size_t) == 4");
#else
    #error "Unsupported platform"
#endif

#endif  // KWCTOOLKIT_BASE_PLATFORM_H_
