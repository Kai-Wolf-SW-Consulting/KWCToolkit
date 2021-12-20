// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "audio/PcmUtils.h"

namespace kwc {
namespace audio {

constexpr float kScale16ToFloat = (1.0f / (1 << 15));

void ConvertFloatToPCM16(const float* source, int16* dest, int32 num_samples) {
    for (int idx = 0; idx < num_samples; ++idx) {
        auto fval = source[idx];
        fval += 1.0f;
        fval *= 1 << 15;
        auto sample = static_cast<int32_t>(fval);
        if (sample < 0)
            sample = 0;
        else if (sample > (1 << 16) - 1)
            sample = (1 << 16) - 1;
        sample -= 1 << 15;  // center at zero
        dest[idx] = static_cast<int16_t>(sample);
    }
}

void ConvertPCM16ToFloat(const int16* source, float* dest, int32 num_samples) {
    for (int idx = 0; idx < num_samples; ++idx) {
        dest[idx] = source[idx] * kScale16ToFloat;
    }
}

}  // namespace audio
}  // namespace kwc
