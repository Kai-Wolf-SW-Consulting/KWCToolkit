// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "base/Assert.h"
#include "base/ScopeGuard.h"
#include "system/SystemMemoryInfo.h"

#include <mach/host_info.h>
#include <mach/i386/vm_param.h>
#include <mach/kern_return.h>
#include <mach/mach_host.h>
#include <mach/mach_init.h>
#include <mach/mach_port.h>
#include <mach/message.h>
#include <mach/vm_statistics.h>

namespace kwc {
namespace system {

bool GetSystemMemoryInfo(SystemMemoryInfo *mem_info) {
    struct host_basic_info host_basic_info;
    mach_msg_type_number_t count = HOST_BASIC_INFO_COUNT;

    // Handle Mach ports, since they are ref counted and need to get back to kernel space
    // once we are done with it
    auto host = mach_host_self ();
    KWC_SCOPE_EXIT
    {
        mach_port_deallocate (mach_task_self (), host);
    };

    int result = host_info (host, HOST_BASIC_INFO, reinterpret_cast<host_info_t> (&host_basic_info),
                            &count);
    if (result != KERN_SUCCESS)
    {
        return false;
    }

    KWC_ASSERT (HOST_BASIC_INFO_COUNT == count);
    mem_info->total = static_cast<int> (host_basic_info.max_mem / 1024);

    vm_statistics64_data_t vmInfo;
    count = HOST_VM_INFO64_COUNT;

    if (host_statistics64 (host, HOST_VM_INFO64, reinterpret_cast<host_info64_t> (&vmInfo),
                           &count) != KERN_SUCCESS)
    {
        return false;
    }
    KWC_ASSERT (HOST_VM_INFO64_COUNT == count);
    KWC_ASSERT (PAGE_SIZE % 1024 == 0 && "Invalid page size");

    mem_info->free = static_cast<int> (PAGE_SIZE / 1024 *
        (vmInfo.free_count - vmInfo.speculative_count));
    mem_info->speculative = static_cast<int> (PAGE_SIZE / 1024 * vmInfo.speculative_count);
    mem_info->file_backed = static_cast<int> (PAGE_SIZE / 1024 * vmInfo.external_page_count);
    mem_info->purgeable = static_cast<int> (PAGE_SIZE / 1024 * vmInfo.purgeable_count);

    return true;
}

}
}