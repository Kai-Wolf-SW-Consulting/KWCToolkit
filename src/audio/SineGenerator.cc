// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "audio/SineGenerator.h"

namespace kwc {
namespace audio {

struct Default {
    static constexpr int FrameRate = 48000;
    static constexpr double Amplitude = 0.01;
    static constexpr double Frequency = 440.0;
    static constexpr double TwoPi = M_PI * 2;
};

SineGenerator::SineGenerator() {
    setup(Default::Frequency, Default::FrameRate, Default::Amplitude);
}

void SineGenerator::setup(double frequency, int32 frameRate) {
    frame_rate_ = frameRate;
    phase_increment_ = getPhaseIncrement(frequency);
}

void SineGenerator::setup(double frequency, int32 frameRate, double amplitude) {
    setup(frequency, frameRate);
    amplitude_ = amplitude;
}

void SineGenerator::render(double* buffer, int32 channelStride, int32 numFrames) {
    for (int idx = 0, sampleIndex = 0; idx < numFrames; ++idx) {
        buffer[sampleIndex] = static_cast<double>(std::sin(phase_) * amplitude_);
        sampleIndex += channelStride;
        advancePhase();
    }
}

void SineGenerator::advancePhase() {
    phase_ += phase_increment_;
    while (phase_ >= Default::TwoPi) {
        phase_ -= Default::TwoPi;
    }
}

double SineGenerator::getPhaseIncrement(double frequency) {
    return frequency * Default::TwoPi / frame_rate_;
}

}  // namespace audio
}  // namespace kwc
