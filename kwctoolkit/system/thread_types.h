// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_SYSTEM_THREAD_TYPES_H_
#define KWCTOOLKIT_SYSTEM_THREAD_TYPES_H_

#include "kwctoolkit/base/platform.h"

#if defined(KWC_OS_WINDOWS)
    // The inclusion of winsock2.h before windows.h is necessary here in order
    // to maintain consistency with win32 specifics
    #define _WINSOCKAPI_
    #include <windows.h>
    #include <winsock2.h>
#elif defined(KWC_OS_POSIX)
    #include <pthread.h>
    #include <unistd.h>
    #if defined(KWC_OS_MACOS)
        #include <pthread_spis.h>
    #endif
#endif

namespace kwc {
namespace system {
#if defined(KWC_OS_WINDOWS)
using PlatformThreadId = DWORD;
using PlatformThreadRef = DWORD;
#elif defined(KWC_OS_POSIX)
using PlatformThreadId = pid_t;
using PlatformThreadRef = pthread_t;
#endif

// Get the ID of the current thread
PlatformThreadId CurrentThreadId();

// Get a reference to the current thread.
// On Windows this is the same as CurrentThreadId. On other platforms it's
// the pthread_t returned by pthread_self()
PlatformThreadRef CurrentThreadRef();

// Compare two threads for equality
bool IsThreadRefEqual(const PlatformThreadRef& a, const PlatformThreadRef& b);

// Set the current thread name
void SetCurrentThreadName(const char* name);

}  // namespace system
}  // namespace kwc

#endif  // KWCTOOLKIT_SYSTEM_THREAD_TYPES_H_
