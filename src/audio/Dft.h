// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_AUDIO_DFT_H_
#define KWCTOOLKIT_AUDIO_DFT_H_

#include <cmath>
#include <complex>
#include <vector>

#ifndef M_PI
    #define M_PI (3.14159265358979323846)
#endif

namespace kwc {
namespace audio {

template <typename T>
std::vector<std::complex<T>> DFT(const std::vector<std::complex<T>>& input) {
    auto n = static_cast<int>(input.size());
    std::vector<std::complex<T>> output;
    for (int k = 0; k < n; ++k) {
        std::complex<T> sum{0};
        for (int t = 0; t < n; ++t) {
            T angle = (-2 * M_PI) / n * (static_cast<long long>(t) * k % n);
            sum += input[t] * std::exp(std::complex<T>{0, angle});
        }
        output.push_back(sum);
    }
    return output;
}

template <typename T>
std::vector<std::complex<T>> DFT(const std::vector<T>& input) {
    auto n = static_cast<int>(input.size());
    std::vector<std::complex<T>> output;
    for (int k = 0; k < n; ++k) {
        T sum_real{0}, sum_imag{0};
        for (int t = 0; t < n; ++t) {
            T angle = (-2 * M_PI) / n * (static_cast<long long>(t) * k % n);
            sum_real += input[t] * std::cos(angle);
            sum_imag += input[t] * std::sin(angle);
        }
        output.emplace_back(sum_real, sum_imag);
    }
    return output;
}

template <typename T>
std::vector<std::complex<T>> DFTInverse(const std::vector<std::complex<T>>& input) {
    auto n = static_cast<int>(input.size());
    std::vector<std::complex<T>> output;
    for (int k = 0; k < n; ++k) {
        std::complex<T> sum{0};
        for (int t = 0; t < n; ++t) {
            T angle = (2 * M_PI) / n * (static_cast<long long>(t) * k % n);
            sum += input[t] * std::exp(std::complex<T>{0, angle});
        }
        output.push_back(sum);
    }
    return output;
}

template <typename T>
std::vector<std::complex<T>> DFTInverse(const std::vector<T>& input) {
    auto n = static_cast<int>(input.size());
    std::vector<std::complex<T>> output;
    for (int k = 0; k < n; ++k) {
        T sum_real{0}, sum_imag{0};
        for (int t = 0; t < n; ++t) {
            T angle = (2 * M_PI) / n * (static_cast<long long>(t) * k % n);
            sum_real += input[t] * std::cos(angle);
            sum_imag += input[t] * std::sin(angle);
        }
        output.emplace_back(sum_real, sum_imag);
    }
    return output;
}

}  // namespace audio
}  // namespace kwc

#endif  // KWCTOOLKIT_AUDIO_DFT_H_
