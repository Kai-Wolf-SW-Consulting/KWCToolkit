// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWC_SYSTEM_TIMER_H_
#define KWC_SYSTEM_TIMER_H_

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

namespace kwc {
namespace system {

// Stopwatch like timer class for benchmarking purposes
//
// This class implements stopwatch timer functionality and dispatches on platform specific
// implementations to provide the highest accuracy possible
class Timer {
  public:
    virtual ~Timer() {}

    // Stopwatch timer functionality
    // Use start () and stop () to record the duration and use getElapsedTime ()
    // to query the duration. If getElapsedTime () is called in between, it will
    // simply report the elapsed time since start ().
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual double getElapsedTime() const = 0;

    // Timestamp functionality
    // Use this function to get an absolute time with an unknown origin.
    // Note that this time moves forward regardless (monotonically increasing)
    virtual double getAbsoluteTime() = 0;
};

// Create UTF ISO8601 formatted timestamp
inline std::string getISO8601TimeUTC() {
    const auto now = std::chrono::system_clock::now();
    const auto itt = std::chrono::system_clock::to_time_t(now);

#if defined(__GNUC__) && __GNUC__ >= 5
    std::ostringstream ss;
    ss << std::put_time(std::gmtime(&itt), "%FT%TZ");
    return ss.str();
#else
    char timeBuffer[80];
    std::strftime(&timeBuffer[0], 80, "%FT%TZ", std::localtime(&itt));
    return timeBuffer;
#endif
}

Timer* createTimer();

}  // namespace system
}  // namespace kwc

#endif  // KWC_SYSTEM_TIMER_H_