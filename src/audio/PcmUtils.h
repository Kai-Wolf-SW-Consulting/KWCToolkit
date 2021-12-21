// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_AUDIO_PCM_UTILS_H_
#define KWCTOOLKIT_AUDIO_PCM_UTILS_H_

#include <sys/types.h>
#include <unistd.h>

#include "base/IntegralTypes.h"

namespace kwc {
namespace audio {

void ConvertFloatToPCM16(const float* source, int16* dest, int32 num_samples);
void ConvertPCM16ToFloat(const int16* source, float* dest, int32 num_samples);

}  // namespace audio
}  // namespace kwc

#endif  // KWCTOOLKIT_AUDIO_PCM_U_TILS_H_
