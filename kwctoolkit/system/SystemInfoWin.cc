// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/base/Assert.h"
#include "kwctoolkit/base/Macros.h"
#include "kwctoolkit/system/SystemInfo.h"
#include "kwctoolkit/system/SystemMemoryInfo.h"

#define NOMINMAX
#include <windows.h>

#include <limits>
#include <sstream>

namespace kwc {
namespace system {

namespace {
int64 GetAmountOfMemory(DWORDLONG MEMORYSTATUSEX::*memoryField) {  // NOLINT
    MEMORYSTATUSEX mem_info;
    mem_info.dwLength = sizeof(mem_info);
    if (!GlobalMemoryStatusEx(&mem_info)) {
        KWC_UNREACHABLE();
        return 0;
    }

    const auto rv = static_cast<int64>(mem_info.*memoryField);
    return rv < 0 ? std::numeric_limits<int64>::max() : rv;
}

const _SYSTEM_INFO& GetSystemInfo() {
    _SYSTEM_INFO info = {};
    ::GetNativeSystemInfo(&info);  // NOLINT
    return info;
}

struct OSInfo {
    struct VersionNumber {
        int major;
        int minor;
        int build;
    };

    struct ServicePack {
        int major;
        int minor;
    };

    // The native processor architecture this version Windows is using
    enum WindowsArchitecture {
        X86_ARCHITECTURE,
        X64_ARCHITECTURE,
        IA64_ARCHITECTURE,
        ARM64_ARCHITECTURE,
        OTHER_ARCHITECTURE
    };

    static OSInfo* getInstance() { return *getInstanceStorage(); }

    const int& getNumberOfCPUs() const { return cpus_; }

    const VersionNumber& getVersionNumber() const { return version_number_; }

    const ServicePack getServicePack() const { return service_pack_; }

    static WindowsArchitecture getArchitecture() {
        switch (GetSystemInfo().wProcessorArchitecture) {
            case PROCESSOR_ARCHITECTURE_INTEL: return X86_ARCHITECTURE;
            case PROCESSOR_ARCHITECTURE_AMD64: return X64_ARCHITECTURE;
            case PROCESSOR_ARCHITECTURE_IA64: return IA64_ARCHITECTURE;
            case PROCESSOR_ARCHITECTURE_ARM64: return ARM64_ARCHITECTURE;
            default: return OTHER_ARCHITECTURE;
        }
    }

  private:
    OSInfo(const _OSVERSIONINFOEXW& version_info, const _SYSTEM_INFO& system_info, int) {
        version_number_.major = version_info.dwMajorVersion;
        version_number_.minor = version_info.dwMinorVersion;
        version_number_.build = version_info.dwBuildNumber;

        service_pack_.major = version_info.wServicePackMajor;
        service_pack_.minor = version_info.wServicePackMinor;

        cpus_ = system_info.dwNumberOfProcessors;
    }

    static OSInfo** getInstanceStorage() {
        static OSInfo* info = []() {
            _OSVERSIONINFOEXW version_info = {sizeof(version_info)};
            ::GetVersionExW(reinterpret_cast<_OSVERSIONINFOW*>(&version_info));

            DWORD os_type = 0;
            ::GetProductInfo(version_info.dwMajorVersion, version_info.dwMinorVersion, 0, 0,
                             &os_type);

            return new OSInfo(version_info, GetSystemInfo(), os_type);
        }();

        return &info;
    }

    VersionNumber version_number_;
    ServicePack service_pack_;
    int cpus_;

    DISALLOW_COPY_AND_ASSIGN(OSInfo);
};
}  // namespace

int SystemInfo::getNumberOfCPUs() {
    return OSInfo::getInstance()->getNumberOfCPUs();
}

std::string SystemInfo::getOSName() {
    return "Windows";
}

std::string SystemInfo::getOSVersion() {
    auto* os_info = OSInfo::getInstance();
    auto version_number = os_info->getVersionNumber();

    std::ostringstream buffer;
    buffer << version_number.major << "." << version_number.minor << "." << version_number.build;

    auto service_pack = os_info->getServicePack();
    if (service_pack.major != 0) {
        buffer << " SP" << service_pack.major;
        if (service_pack.minor != 0) {
            buffer << "." << service_pack.minor;
        }
    }
    return buffer.str();
}

std::string SystemInfo::getOSArch() {
    const auto arch = OSInfo::getArchitecture();
    switch (arch) {
        case OSInfo::X86_ARCHITECTURE: return "x86";
        case OSInfo::X64_ARCHITECTURE: return "x86_64";
        case OSInfo::IA64_ARCHITECTURE: return "ia64";
        case OSInfo::ARM64_ARCHITECTURE: return "arm64";
        default: return "";
    }
}

int64 SystemInfo::getAmountOfPhysicalMemoryImpl() {
    return GetAmountOfMemory(&MEMORYSTATUSEX::ullTotalPhys);
}

int64 SystemInfo::getAmountOfAvailablePhysicalMemoryImpl() {
    SystemMemoryInfo mem_info;
    if (!GetSystemMemoryInfo(&mem_info)) {
        return 0;
    }

    return static_cast<int64>(mem_info.avail_phys) * 1024;
}

int64 SystemInfo::getAmountOfVirtualMemory() {
    return GetAmountOfMemory(&MEMORYSTATUSEX::ullTotalVirtual);
}

}  // namespace system
}  // namespace kwc