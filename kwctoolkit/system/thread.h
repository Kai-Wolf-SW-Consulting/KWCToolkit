// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_SYSTEM_THREAD_H_
#define KWCTOOLKIT_SYSTEM_THREAD_H_

#include "kwctoolkit/base/macros.h"
#include "kwctoolkit/base/platform.h"

#if defined(KWC_OS_POSIX)
    #include <pthread.h>
#endif
#include <string>

#include "kwctoolkit/system/thread_types.h"

namespace kwc {
namespace system {
// Callback function that the spawned thread will run once spawned
using ThreadRunFunction = void (*)(void*);

enum ThreadPriority {
#if defined(KWC_OS_WINDOWS)
    LOW_PRIORITY = THREAD_PRIORITY_BELOW_NORMAL,
    NORMAL_PRIORITY = THREAD_PRIORITY_NORMAL,
    HIGH_PRIORITY = THREAD_PRIORITY_ABOVE_NORMAL,
    HIGHEST_PRIORITY = THREAD_PRIORITY_HIGHEST,
    REALTIME_PRIORITY = THREAD_PRIORITY_TIME_CRITICAL
#else
    LOW_PRIORITY = 1,
    NORMAL_PRIORITY = 2,
    HIGH_PRIORITY = 3,
    HIGHEST_PRIORITY = 4,
    REALTIME_PRIORITY = 5
#endif
};

// Implements a simple worker thread
//
// This version of a thread is used above C++11 std::thread version to avoid
// several flaws that come with the standard's implementation.
// For one, C++11 threads always start_ immediately. While this can be avoided,
// jumping through several hoops (using condition variables etc.), ideally
// our implementation provides a more cleaner start_()/stop() semantic.
//
// Furthermore, our implementation also provides a method to attach a name
// to a thread, which is handy in debugging environments as well as for
// stack traces.
//
// This implementation is mainly a wrapper around Windows' threads and
// pthread for POSIX.
class Thread {
  public:
    Thread(ThreadRunFunction func,
           void* obj,
           const char* thread_name,
           ThreadPriority priority = NORMAL_PRIORITY);
    virtual ~Thread();

    const std::string& name() const;

    // Spawns a thread and tries to set thread priority accordingly
    void start();

    bool isRunning() const;

    // Returns an identifier for the worker thread
    PlatformThreadRef getThreadRef() const;

    // Stops (joins) the spawned thread
    void stop();

  private:
    DISALLOW_COPY_AND_ASSIGN(Thread);

    void run();
    bool setPriority(ThreadPriority priority);

    ThreadRunFunction const run_function_ = nullptr;
    const ThreadPriority priority_ = NORMAL_PRIORITY;
    void* const obj_;
    const std::string name_;

#if defined(KWC_OS_WINDOWS)
    static DWORD WINAPI startThread(void* param);
    HANDLE thread_ = nullptr;
    DWORD thread_id_ = 0;
#else
    static void* startThread(void* param);
    pthread_t thread_ = nullptr;
#endif
};

}  // namespace system
}  // namespace kwc

#endif  // KWCTOOLKIT_SYSTEM_THREAD_H_
