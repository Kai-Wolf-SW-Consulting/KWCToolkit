// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include <windows.h>
#include <psapi.h>
#include <winternl.h>

#include <cstddef>
#include <cstdint>

#include "system/system_memory_info.h"

namespace kwc {
namespace system {

bool GetSystemMemoryInfo(SystemMemoryInfo* mem_info) {
    MEMORYSTATUSEX mem_status;
    mem_status.dwLength = sizeof(mem_status);
    if (!::GlobalMemoryStatusEx(&mem_status)) {
        return false;
    }

    mem_info->total = mem_status.ullTotalPhys / 1024;
    mem_info->avail_phys = mem_status.ullAvailPhys / 1024;
    mem_info->swap_total = mem_status.ullTotalPageFile / 1024;
    mem_info->swap_free = mem_status.ullAvailPageFile / 1024;

    return true;
}

}  // namespace system
}  // namespace kwc