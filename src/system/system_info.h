// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_SYSTEM_SYSTEM_INFO_H_
#define KWCTOOLKIT_SYSTEM_SYSTEM_INFO_H_

#include <string>

#include "base/integral_types.h"
#include "base/platform.h"              // IWYU pragma: keep
#include "system/system_memory_info.h"  // IWYU pragma: keep

namespace kwc {
namespace system {

class SystemInfo {
  public:
    // Return the number of logical processors on this machine
    static int getNumberOfCPUs();

    // Return the number of bytes of physical memory on this machine
    static int64 getAmountOfPhysicalMemory();

    // Return the number of bytes of current available physical memory on this machine
    // That is, the amount that can be allocated without any significant runtime impact on the
    // system
    static int64 getAmountOfAvailablePhysicalMemory();

    // Return the number of megabytes of physical memory on this machine
    static int getAmountOfPhysicalMemoryMB();

    // Return the number of bytes of virtual memory for this process
    static int64 getAmountOfVirtualMemory();

    // Return the number of megabytes of virtual memory for this process
    static int getAmountOfVirtualMemoryMB();

    // Return the name of the host operating system
    static std::string getOSName();

    // Return the version of the host operating system
    static std::string getOSVersion();

    // Return the architecture of the host operating system
    static std::string getOSArch();

  private:
    static int64 getAmountOfPhysicalMemoryImpl();
    static int64 getAmountOfAvailablePhysicalMemoryImpl();

#if defined(OS_LINUX)
    static int64 getAmountOfAvailablePhysicalMemory(const SystemMemoryInfo& mem_info);
#endif
};

}  // namespace system
}  // namespace kwc

#endif  // KWCTOOLKIT_SYSTEM_SYSTEM_INFO_H_
