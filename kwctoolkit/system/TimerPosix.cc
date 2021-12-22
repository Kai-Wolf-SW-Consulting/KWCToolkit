// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include <cstdint>
#include <ctime>

#include <kwctoolkit/base/IntegralTypes.h>
#include <kwctoolkit/system/Timer.h>

namespace kwc {
namespace system {

namespace {
uint64 getCurrentTimeNanos() {
    struct timespec currentTime;
    clock_gettime(CLOCK_MONOTONIC, &currentTime);
    return currentTime.tv_sec * 1000000000llu + currentTime.tv_nsec;
}
}  // namespace

class TimerPosix : public Timer {
  public:
    TimerPosix() : Timer(), running_(false) {}

    ~TimerPosix() override = default;

    void start() override {
        start_time_nanos_ = getCurrentTimeNanos();
        running_ = true;
    }

    void stop() override {
        stop_time_nanos_ = getCurrentTimeNanos();
        running_ = false;
    }

    double getElapsedTime() const override {
        uint64 endTimeNanos;
        if (running_) {
            endTimeNanos = getCurrentTimeNanos();
        } else {
            endTimeNanos = stop_time_nanos_;
        }

        return (endTimeNanos - start_time_nanos_) * 1e-9;
    }

    double getAbsoluteTime() override { return getCurrentTimeNanos() * 1e-9; }

  private:
    bool running_;
    uint64 start_time_nanos_;
    uint64 stop_time_nanos_;
};

Timer* createTimer() {
    return new TimerPosix();
}

}  // namespace system
}  // namespace kwc