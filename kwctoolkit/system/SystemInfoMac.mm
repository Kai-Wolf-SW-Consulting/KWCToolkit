// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#import <Foundation/Foundation.h>
#include <mach/host_info.h>
#include <mach/kern_return.h>
#include <mach/mach_host.h>
#include <mach/mach_init.h>
#include <mach/mach_port.h>
#include <mach/message.h>

#include <sstream>
#include <string>

#include "kwctoolkit/base/Assert.h"
#include "kwctoolkit/base/IntegralTypes.h"
#include "kwctoolkit/base/ScopeGuard.h"
#include "kwctoolkit/system/SystemInfo.h"

namespace kwc {
namespace system {

std::string SystemInfo::getOSName() {
    return "macOS";
}

std::string SystemInfo::getOSVersion() {
    NSOperatingSystemVersion version = [[NSProcessInfo processInfo] operatingSystemVersion];
    auto major = version.majorVersion;
    auto minor = version.minorVersion;
    auto patch = version.patchVersion;
    std::ostringstream buffer;
    buffer << major << "." << minor << "." << patch;
    return buffer.str();
}

int64 SystemInfo::getAmountOfPhysicalMemoryImpl() {
    struct host_basic_info hostInfo;
    mach_msg_type_number_t count = HOST_BASIC_INFO_COUNT;

    // Handle Mach port that names a send right, since they are ref counted and need to get back
    // to kernel space one we are done with it
    auto host = mach_host_self();
    KWC_SCOPE_EXIT { mach_port_deallocate(mach_task_self(), host); };

    int result = host_info(host, HOST_BASIC_INFO, reinterpret_cast<host_info_t>(&hostInfo), &count);
    if (result != KERN_SUCCESS) {
        KWC_UNREACHABLE();
        return 0;
    }

    KWC_ASSERT(HOST_BASIC_INFO_COUNT == count);
    return static_cast<int64>(hostInfo.max_mem);
}

int64 SystemInfo::getAmountOfAvailablePhysicalMemoryImpl() {
    SystemMemoryInfo mem_info;
    if (!GetSystemMemoryInfo(&mem_info)) {
        return 0;
    }

    return static_cast<int64>(mem_info.free + mem_info.speculative) * 1024;
}

}  // namespace system
}  // namespace kwc
