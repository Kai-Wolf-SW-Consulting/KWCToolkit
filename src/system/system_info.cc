// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "system/system_info.h"

namespace kwc {
namespace system {

int64 SystemInfo::getAmountOfPhysicalMemory() {
    return getAmountOfPhysicalMemoryImpl();
}

int64 SystemInfo::getAmountOfAvailablePhysicalMemory() {
    return getAmountOfAvailablePhysicalMemoryImpl();
}

int SystemInfo::getAmountOfPhysicalMemoryMB() {
    return static_cast<int>(getAmountOfPhysicalMemory() / 1024 / 1024);
}

int SystemInfo::getAmountOfVirtualMemoryMB() {
    return static_cast<int>(getAmountOfVirtualMemory() / 1024 / 1024);
}

}  // namespace system
}  // namespace kwc
