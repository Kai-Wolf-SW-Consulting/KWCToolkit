// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_SYSTEM_SLEEP_H_
#define KWCTOOLKIT_SYSTEM_SLEEP_H_

#include <kwctoolkit/base/IntegralTypes.h>

namespace kwc {
namespace system {

const int64 kNumMillisPerSecond = 1000LL;
const int64 kNumMicrosPerMillis = 1000LL;
const int64 kNumMicrosPerSecond = kNumMillisPerSecond * 1000LL;
const int64 kNumNanosPerMicros = 1000LL;
const int64 kNumNanosPerSecond = kNumNanosPerMicros * kNumMicrosPerSecond;

void SleepForMilliseconds(int millis);
void SleepForSeconds(double seconds);

}  // namespace system
}  // namespace kwc

#endif  // KWCTOOLKIT_SYSTEM_SLEEP_H_
