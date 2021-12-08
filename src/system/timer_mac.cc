// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include <CoreServices/CoreServices.h>
#include <mach/mach.h>
#include <mach/mach_time.h>

#include <cstdint>

#include "base/integral_types.h"
#include "system/timer.h"

namespace kwc {
namespace system {

class TimerMac : public Timer {
  public:
    TimerMac() : Timer(), running_(false), sec_coeff_(0.0) {}

    ~TimerMac() override = default;

    void start() override {
        start_time_ = mach_absolute_time();
        getSecondCoeff();
        running_ = true;
    }

    void stop() override {
        stop_time_ = mach_absolute_time();
        running_ = false;
    }

    double getElapsedTime() const override {
        if (running_) {
            return sec_coeff_ * (mach_absolute_time() - start_time_);
        } else {
            return sec_coeff_ * (stop_time_ - start_time_);
        }
    }

    double getAbsoluteTime() override { return getSecondCoeff() * mach_absolute_time(); }

  private:
    double getSecondCoeff() {
        if (sec_coeff_ == 0.0) {
            mach_timebase_info_data_t tbInfo;
            mach_timebase_info(&tbInfo);

            sec_coeff_ = tbInfo.numer * (1.0 / 1000000000) / tbInfo.denom;
        }

        return sec_coeff_;
    }

    bool running_;
    uint64 start_time_;
    uint64 stop_time_;
    double sec_coeff_;
};

Timer* createTimer() {
    return new TimerMac();
}

}  // namespace system
}  // namespace kwc