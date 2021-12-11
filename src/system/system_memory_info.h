// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_SYSTEM_SYSTEM_MEMORY_INFO_H_
#define KWCTOOLKIT_SYSTEM_SYSTEM_MEMORY_INFO_H_

#include "base/platform.h"

namespace kwc {
namespace system {

// Data structure for system memory consumption
struct SystemMemoryInfo {
    SystemMemoryInfo() = default;
    SystemMemoryInfo(const SystemMemoryInfo& other) = default;

    int total{0};
#if !defined(OS_WINDOWS)
    int free{0};
#endif

#if defined(OS_WINDOWS)
    // the amount of physical memory that can be immediately reused without having to write its
    // content to disk first.
    // See: https://docs.microsoft.com/en-us/windows/win32/api/winbase/ns-winbase-memorystatus
    int avail_phys{0};
#endif

#if defined(OS_LINUX) || defined(OS_ANDROID)
    // Provides an estimate of available memory
    // See: https://git.kernel.org/cgit/linux/kernel/git/torvalds/linux.git/commit/?id=34e431b0ae
    int available{0};
#endif

#if !defined(OS_MACOS)
    int swap_total{0};
    int swap_free{0};
#endif

#if defined(OS_ANDROID) || defined(OS_LINUX)
    int buffers{0};
    int cached{0};
    int active_anon{0};
    int inactive_anon{0};
    int active_file{0};
    int inactive_file{0};
    int dirty{0};
    int reclaimable{0};
#endif

#if defined(OS_MACOS)
    int speculative{0};
    int file_backed{0};
    int purgeable{0};
#endif
};

// Parse memory consumption from either /proc/meminfo and /proc/vmstat (Linux, Android) or retrieve
// it using system API calls (Windows, macOS)
bool GetSystemMemoryInfo(SystemMemoryInfo* mem_info);

}  // namespace system
}  // namespace kwc

#endif  // KWCTOOLKIT_SYSTEM_SYSTEM_MEMORY_INFO_H_
