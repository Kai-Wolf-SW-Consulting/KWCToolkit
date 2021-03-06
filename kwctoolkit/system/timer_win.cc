// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/base/windows.h"
#include "kwctoolkit/system/timer.h"

namespace kwc {
namespace system {

class TimerWin : public Timer {
  public:
    TimerWin() : Timer(), running_(false), frequency_(0) {}

    void start() override {
        LARGE_INTEGER current_time;
        QueryPerformanceCounter(&current_time);
        start_time_ = current_time.QuadPart;

        getFrequency();
        running_ = true;
    }

    void stop() override {
        LARGE_INTEGER current_time;
        QueryPerformanceCounter(&current_time);
        stop_time_ = current_time.QuadPart;
        running_ = false;
    }

    double getElapsedTime() const override {
        LONGLONG end_time;
        if (running_) {
            LARGE_INTEGER current_time;
            QueryPerformanceCounter(&current_time);
            end_time = current_time.QuadPart;
        } else {
            end_time = stop_time_;
        }

        return static_cast<double>(end_time - start_time_) / frequency_;
    }

    double getAbsoluteTime() override {
        LARGE_INTEGER current_time;
        QueryPerformanceCounter(&current_time);
        return static_cast<double>(current_time.QuadPart) / getFrequency();
    }

  private:
    LONGLONG getFrequency() {
        if (frequency_ == 0) {
            LARGE_INTEGER frequency = {};
            QueryPerformanceFrequency(&frequency);
            frequency_ = frequency.QuadPart;
        }

        return frequency_;
    }

    bool running_;
    LONGLONG start_time_;
    LONGLONG stop_time_;
    LONGLONG frequency_;
};

Timer* CreateTimer() {
    return new TimerWin();
}

}  // namespace system
}  // namespace kwc