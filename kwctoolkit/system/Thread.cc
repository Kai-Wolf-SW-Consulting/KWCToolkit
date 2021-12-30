// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/system/Thread.h"

#include <algorithm>

#include "kwctoolkit/base/Assert.h"
#include "kwctoolkit/base/Platform.h"  // IWYU pragma: keep

#if !defined(KWC_OS_WINDOWS)
    #include <pthread.h>
    #include <sched.h>
#endif

namespace kwc {
namespace system {
namespace {
#if !defined(KWC_OS_WINDOWS)
struct ThreadAttributes {
    ThreadAttributes() { pthread_attr_init(&attr); }

    ~ThreadAttributes() { pthread_attr_destroy(&attr); }

    pthread_attr_t* operator&() { return &attr; }

    pthread_attr_t attr;
};
#endif
}  // namespace

Thread::Thread(ThreadRunFunction func, void* obj, const char* thread_name, ThreadPriority priority)
    : run_function_(func), priority_(priority), obj_(obj), name_(thread_name) {
    KWC_ASSERT(func);
    KWC_ASSERT(!name_.empty());
    KWC_ASSERT(name_.length() < 64 && "Thread name too long");
}

Thread::~Thread() {
#if defined(KWC_OS_WINDOWS)
    KWC_ASSERT(!thread_);
    KWC_ASSERT(!thread_id_);
#endif
}

const std::string& Thread::name() const {
    return name_;
}

void Thread::start() {
    KWC_ASSERT(!thread_ && "Thread already started");
#if defined(KWC_OS_WINDOWS)
    // Set the reserved stack size to 1M, which is default on Windows/Linux
    thread_ = ::CreateThread(nullptr, 1024 * 1024, &startThread, this,
                             STACK_SIZE_PARAM_IS_A_RESERVATION, &thread_id_);
    KWC_ASSERT(thread_ && "Thread creation failed");
    KWC_ASSERT(thread_id_);
#else
    ThreadAttributes attr;
    // Set the stack size to 1M
    pthread_attr_setstacksize(&attr, 1024 * 1024);
    KWC_ASSERT(pthread_create(&thread_, &attr, &startThread, this) == 0 &&
               "Thread creation failed");
#endif
}

bool Thread::isRunning() const {
#if defined(KWC_OS_WINDOWS)
    return thread_ != nullptr;
#else
    return thread_ != nullptr;
#endif
}

PlatformThreadRef Thread::getThreadRef() const {
#if defined(KWC_OS_WINDOWS)
    return thread_id_;
#else
    return thread_;
#endif
}

void Thread::stop() {
    if (!isRunning()) {
        return;
    }

#if defined(KWC_OS_WINDOWS)
    WaitForSingleObject(thread_, INFINITE);
    CloseHandle(thread_);
    thread_ = nullptr;
    thread_id_ = 0;
#else
    KWC_ASSERT(pthread_join(thread_, nullptr) == 0);
    thread_ = nullptr;
#endif
}

void Thread::run() {
    SetCurrentThreadName(name_.c_str());
    setPriority(priority_);
    run_function_(obj_);
}

bool Thread::setPriority(ThreadPriority priority) {
#if defined(KWC_OS_WINDOWS)
    return SetThreadPriority(thread_, priority) != FALSE;
#else
    const int policy = SCHED_FIFO;
    const int min_priority = sched_get_priority_min(policy);
    const int max_priority = sched_get_priority_max(policy);
    if (min_priority == -1 || max_priority == -1) {
        return false;
    }

    if (max_priority - min_priority <= 2) {
        return false;
    }

    sched_param param{};
    const int top_priority = max_priority - 1;
    const int low_priority = min_priority + 1;
    switch (priority) {
        case LOW_PRIORITY: param.sched_priority = low_priority; break;
        case NORMAL_PRIORITY: param.sched_priority = (low_priority + top_priority - 1) / 2; break;
        case HIGH_PRIORITY: param.sched_priority = std::max(top_priority - 2, low_priority); break;
        case HIGHEST_PRIORITY:
            param.sched_priority = std::max(top_priority - 1, low_priority);
            break;
        case REALTIME_PRIORITY: param.sched_priority = top_priority; break;
    }
    return pthread_setschedparam(thread_, policy, &param) == 0;
#endif
}

#if defined(KWC_OS_WINDOWS)
DWORD WINAPI Thread::startThread(void* param) {
    ::SetLastError(ERROR_SUCCESS);
    static_cast<Thread*>(param)->run();
    return 0;
}
#else
void* Thread::startThread(void* param) {
    static_cast<Thread*>(param)->run();
    return nullptr;
}
#endif

}  // namespace system
}  // namespace kwc
