// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "system/ThreadTypes.h"

#include "base/Platform.h"

#if defined(OS_LINUX)
    #include <sys/prctl.h>
    #include <sys/syscall.h>
#endif

namespace kwc {
namespace system {

PlatformThreadId currentThreadId() {
#if defined(OS_WINDOWS)
    return GetCurrentThreadId();
#elif defined(OS_POSIX)
    #if defined(OS_MACOS)
    return pthread_mach_thread_np(pthread_self());
    #elif defined(OS_ANDROID)
    return gettid();
    #elif defined(OS_LINUX)
    return syscall(__NR_gettid);
    #else
    return reinterpret_cast < PlatformThreadId(pthread_self());
    #endif
#endif
}

PlatformThreadRef currentThreadRef() {
#if defined(OS_WINDOWS)
    return GetCurrentThreadId();
#elif defined(OS_POSIX)
    return pthread_self();
#endif
}

bool isThreadRefEqual(const PlatformThreadRef& a, const PlatformThreadRef& b) {
#if defined(OS_WINDOWS)
    return a == b;
#elif defined(OS_POSIX)
    return pthread_equal(a, b);
#endif
}

void setCurrentThreadName(const char* name) {
#if defined(OS_WINDOWS)
    // Yes, we are serious. This is the officially documented way to set a thread name in Windows.
    // See the following link for more info on this:
    // http://docs.microsoft.com/en-us/visualstudio/debugger/how-to-set-a-thread-name-in-native-code
    #pragma pack(push, 8)
    struct {
        DWORD dwType;      // Must be 0x1000.
        LPCSTR szName;     // Pointer to name (in user addr space).
        DWORD dwThreadID;  // Thread ID (-1=caller thread).
        DWORD dwFlags;     // Reserved for future use, must be zero.
    } threadname_info = {0x1000, name, static_cast<DWORD>(-1), 0};
    #pragma pack(pop)

    #pragma warning(push)
    #pragma warning(disable : 6320 6322)
    __try {
        ::RaiseException(0x406D1388, 0, sizeof(threadname_info) / sizeof(ULONG_PTR),
                         reinterpret_cast<ULONG_PTR*>(&threadname_info));
    } __except (EXCEPTION_EXECUTE_HANDLER) {}
    #pragma warning(pop)
#elif defined(OS_LINUX) || defined(OS_ANDROID)
    prctl(PR_SET_NAME, reinterpret_cast<unsigned long>(name));
#elif defined(OS_MACOS)
    pthread_setname_np(name);
#endif
}

}  // namespace system
}  // namespace kwc