// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_AUDIO_SINE_GENERATOR_H_
#define KWCTOOLKIT_AUDIO_SINE_GENERATOR_H_

#include <cmath>
#include <cstdint>

#include <kwctoolkit/base/IntegralTypes.h>

namespace kwc {
namespace audio {

class SineGenerator {
  public:
    SineGenerator();
    ~SineGenerator() = default;

    void setup(double frequency, int32 frameRate);
    void setup(double frequency, int32 frameRate, double amplitude);
    void render(double* buffer, int32 channelStride, int32 numFrames);

  private:
    double amplitude_;
    double phase_ = 0.0;
    double phase_increment_;
    int32 frame_rate_;

    void advancePhase();
    double getPhaseIncrement(double frequency);
};

}  // namespace audio
}  // namespace kwc

#endif  // KWCTOOLKIT_AUDIO_SINEGENERATOR_H_
