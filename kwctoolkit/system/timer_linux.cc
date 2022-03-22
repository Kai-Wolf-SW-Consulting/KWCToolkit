// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include <cstdint>
#include <ctime>

#include "kwctoolkit/base/integral_types.h"
#include "kwctoolkit/system/timer.h"

namespace kwc {
namespace system {

namespace {
uint64 GetCurrentTimeNanos() {
    struct timespec current_time;
    clock_gettime(CLOCK_MONOTONIC, &current_time);
    return current_time.tv_sec * 1000000000llu + current_time.tv_nsec;
}
}  // namespace

class TimerPosix : public Timer {
  public:
    TimerPosix() : Timer(), running_(false) {}

    ~TimerPosix() override = default;

    void start() override {
        start_time_nanos_ = GetCurrentTimeNanos();
        running_ = true;
    }

    void stop() override {
        stop_time_nanos_ = GetCurrentTimeNanos();
        running_ = false;
    }

    double getElapsedTime() const override {
        uint64 end_time_nanos;
        if (running_) {
            end_time_nanos = GetCurrentTimeNanos();
        } else {
            end_time_nanos = stop_time_nanos_;
        }

        return (end_time_nanos - start_time_nanos_) * 1e-9;
    }

    double getAbsoluteTime() override { return GetCurrentTimeNanos() * 1e-9; }

  private:
    bool running_;
    uint64 start_time_nanos_;
    uint64 stop_time_nanos_;
};

Timer* CreateTimer() {
    return new TimerPosix();
}

}  // namespace system
}  // namespace kwc