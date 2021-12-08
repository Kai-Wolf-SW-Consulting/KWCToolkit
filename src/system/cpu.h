// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWC_SYSTEM_CPU_H_
#define KWC_SYSTEM_CPU_H_

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

    IntelMicroArchitecture GetIntelMicroArchitecture() const;
    const std::string& vendor() const;
    const std::string& cpu_brand() const;
    int signature() const;
    int stepping() const;
    int model() const;
    int family() const;
    int type() const;
    int extended_model() const;
    int extended_family() const;
    bool has_mmx() const;
    bool has_sse() const;
    bool has_sse2() const;
    bool has_sse3() const;
    bool has_sse41() const;
    bool has_sse42() const;
    bool has_avx() const;
    bool has_avx2() const;

  private:
    // Query the processor for CPUID information
    void Initialize();

    // Raw form of type, family, model and stepping
    int signature_;
    // Processor type
    int type_;
    // Family of the processor
    int family_;
    // Model of the processor
    int model_;
    // Processor revision number
    int stepping_;
    int ext_model_;
    int ext_family_;
    bool has_mmx_;
    bool has_sse_;
    bool has_sse2_;
    bool has_sse3_;
    bool has_sse41_;
    bool has_sse42_;
    bool has_avx_;
    bool has_avx2_;
    std::string cpu_vendor_;
    std::string cpu_brand_;
};

}  // namespace system
}  // namespace kwc

#endif  // KWC_SYSTEM_CPU_H_
