// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "base/assert.h"
#include "system/system_info.h"
#include "system/system_memory_info.h"

#include <unistd.h>

namespace kwc {
namespace system {

int64 SystemInfo::getAmountOfPhysicalMemoryImpl() {
    const auto pages = sysconf(_SC_PHYS_PAGES);
    const auto page_size = sysconf(_SC_PAGESIZE);
    if (pages == -1 || page_size == -1) {
        UNREACHABLE();
        return 0;
    }
    return static_cast<int64>(pages) * page_size;
}

int64 SystemInfo::getAmountOfAvailablePhysicalMemoryImpl() {
    SystemMemoryInfo info;
    if (!GetSystemMemoryInfo(&info)) {
        return 0;
    }

    return getAmountOfAvailablePhysicalMemory(info);
}

int64 SystemInfo::getAmountOfAvailablePhysicalMemory(const SystemMemoryInfo& mem_info) {
    // See: https://git.kernel.org/cgit/linux/kernel/git/torvalds/linux.git/commit/?id=34e431b0ae
    // for more details on this.
    int64 res_kb = mem_info.available != 0
                       ? mem_info.available - mem_info.active_file
                       : mem_info.free + mem_info.reclaimable + mem_info.inactive_file;
    return res_kb * 1024;
}

}  // namespace system
}  // namespace kwc