// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/system/Sleep.h"

#include <cerrno>
#include <ctime>

#include "kwctoolkit/base/Platform.h"

#if defined(KWC_OS_WINDOWS)
    #include <Windows.h>
#endif

namespace kwc {
namespace system {
#if defined(KWC_OS_WINDOWS)
// Windows takes milliseconds argument
void SleepForMilliseconds(int millis) {
    Sleep(millis);
}

void SleepForSeconds(double seconds) {
    SleepForMilliseconds(static_cast<int>(kNumMillisPerSecond * seconds));
}
#else
void SleepForMicroseconds(int micros) {
    struct timespec sleep_time;
    sleep_time.tv_sec = micros / kNumMicrosPerSecond;
    sleep_time.tv_nsec = (micros % kNumMicrosPerSecond) * kNumNanosPerMicros;
    while (nanosleep(&sleep_time, &sleep_time) != 0 && errno == EINTR)
        ;  // Ignore interrupt signals and wait for the full interval to elapse
}

void SleepForMilliseconds(int millis) {
    SleepForMicroseconds(static_cast<int>(millis) * kNumMicrosPerMillis);
}

void SleepForSeconds(double seconds) {
    SleepForMicroseconds(static_cast<int>(seconds * kNumMicrosPerSecond));
}
#endif

}  // namespace system
}  // namespace kwc