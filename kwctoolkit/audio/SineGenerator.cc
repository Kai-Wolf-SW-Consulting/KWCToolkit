// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/audio/SineGenerator.h"

namespace kwc {
namespace audio {

struct Default {
    static constexpr int kFrameRate = 48000;
    static constexpr double kAmplitude = 0.01;
    static constexpr double kFrequency = 440.0;
    static constexpr double kTwoPi = M_PI * 2;
};

SineGenerator::SineGenerator() {
    setup(Default::kFrequency, Default::kFrameRate, Default::kAmplitude);
}

void SineGenerator::setup(double frequency, int32 frame_rate) {
    frame_rate_ = frame_rate;
    phase_increment_ = getPhaseIncrement(frequency);
}

void SineGenerator::setup(double frequency, int32 frame_rate, double amplitude) {
    setup(frequency, frame_rate);
    amplitude_ = amplitude;
}

void SineGenerator::render(double* buffer, int32 channel_stride, int32 num_frames) {
    for (int idx = 0, sample_index = 0; idx < num_frames; ++idx) {
        buffer[sample_index] = static_cast<double>(std::sin(phase_) * amplitude_);
        sample_index += channel_stride;
        advancePhase();
    }
}

void SineGenerator::advancePhase() {
    phase_ += phase_increment_;
    while (phase_ >= Default::kTwoPi) {
        phase_ -= Default::kTwoPi;
    }
}

double SineGenerator::getPhaseIncrement(double frequency) {
    return frequency * Default::kTwoPi / frame_rate_;
}

}  // namespace audio
}  // namespace kwc
