// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_SYSTEM_THREAD_HELPER_H_
#define KWCTOOLKIT_SYSTEM_THREAD_HELPER_H_

#include <unistd.h>

#include <chrono>
#include <future>
#include <thread>
#include <utility>
#include <vector>

#include "kwctoolkit/base/Logging.h"
#include "kwctoolkit/base/Platform.h"

namespace kwc {
namespace system {

// Execute given function |f| with its parames |Ts| truly asynchronously
//
// This helper function acts like std::async, but uses always std::launch::async
// in order to overcome the deficiencies with C++11/14/17 default implementation
template <typename F, typename... Ts>
inline auto RunAsync(F&& f, Ts&&... params) {
    return std::async(std::launch::async, std::forward<F>(f), std::forward<Ts>(params)...);
}

// Checks, if given std::future has executed already
template <typename R>
bool IsReady(const std::future<R>& f, int timeoutMillis = 0) {
    return f.wait_for(std::chrono::milliseconds(timeoutMillis)) == std::future_status::ready;
}

#if defined(KWC_OS_ANDROID)

// Set the thread affinity for the current thread to cpuIDs_
//
// This may be useful to call on the audio thread to avoid buffer underruns
// caused by the CPU core migrations to slower CPU cores.
// See: https://www.youtube.com/watch?v=C0BPXZIvG-Q&t=1158 for more info
inline void SetThreadAffinity(const std::vector<int> cpuIDs) {
    pid_t currentThreadId = gettid();
    cpu_set_t cpu_set{};
    CPU_ZERO(&cpu_set);

    // If the callback CPU IDs are not specified, bind them to the current CPU
    if (cpuIDs.empty()) {
        int currentCPUID = sched_getcpu();
        LOGGING(base::INFO) << "Current CPU ID is: " << currentCPUID;
        CPU_SET(currentCPUID, &cpu_set);
    } else {
        for (const auto cpuID : cpuIDs) {
            LOGGING(base::INFO) << "CPU ID: " << cpuID << " added to cores set";
            CPU_SET(cpuID, &cpu_set);
        }
    }

    auto result = sched_setaffinity(currentThreadId, sizeof(cpu_set_t), &cpu_set);
    if (result == 0) {
        LOGGING(base::INFO) << "Thread affinity is set";
    } else {
        LOGGING(base::WARNING) << "Error setting thread affinity: No. " << result;
    }
}
#endif

}  // namespace system
}  // namespace kwc

#endif  // KWCTOOLKIT_SYSTEM_THREAD_HELPER_H_
