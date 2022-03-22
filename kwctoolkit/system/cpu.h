// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_SYSTEM_CPU_H_
#define KWCTOOLKIT_SYSTEM_CPU_H_

#include <string>

namespace kwc {
namespace system {

class CPU final {
  public:
    CPU();

    enum IntelMicroArchitecture {
        PENTIUM,
        SSE,
        SSE2,
        SSE3,
        SSE41,
        SSE42,
        AVX,
        AVX2,
        MAX_INTEL_MICRO_ARCHITECTURE
    };

    IntelMicroArchitecture getIntelMicroArchitecture() const;
    const std::string& vendor() const;
    const std::string& cpuBrand() const;
    int signature() const;
    int stepping() const;
    int model() const;
    int family() const;
    int type() const;
    int extendedModel() const;
    int extendedFamily() const;
    bool hasMmx() const;
    bool hasSse() const;
    bool hasSse2() const;
    bool hasSse3() const;
    bool hasSse41() const;
    bool hasSse42() const;
    bool hasAvx() const;
    bool hasAvx2() const;

  private:
    // Query the processor for CPUID information
    void initialize();

    // Raw form of type, family, model and stepping
    int signature_{0};
    // Processor type
    int type_{0};
    // Family of the processor
    int family_{0};
    // Model of the processor
    int model_{0};
    // Processor revision number
    int stepping_{0};
    int ext_model_{0};
    int ext_family_{0};
    bool has_mmx_{false};
    bool has_sse_{false};
    bool has_sse2_{false};
    bool has_sse3_{false};
    bool has_sse41_{false};
    bool has_sse42_{false};
    bool has_avx_{false};
    bool has_avx2_{false};
    std::string cpu_vendor_;
    std::string cpu_brand_;
};

}  // namespace system
}  // namespace kwc

#endif  // KWCTOOLKIT_SYSTEM_CPU_H_
