// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include <string>

#include "base/Logging.h"
#include "file/FilePath.h"
#include "file/FileUtils.h"
#include "strings/StringSplit.h"
#include "strings/StringUtils.h"
#include "system/SystemMemoryInfo.h"

namespace kwc {
namespace system {
namespace {
bool ParseProcMemInfo(const std::string& data, SystemMemoryInfo* mem_info) {
    using namespace kwc::strings;

    // The file format of /proc/meminfo is:
    //
    // MemTotal:        1768824 kB
    // MemFree:         1566772 kB
    // MemAvailable:    1636252 kB
    // Buffers:           13132 kB
    // Cached:            67076 kB
    // ...
    // There is no guarantee of the concrete ordering for all entries
    mem_info->total = 0;
    for (const auto& line : SplitString(data, "\n", WHITESPACE_KEEP, SPLIT_WANT_NONEMPTY)) {
        auto tokens = SplitString(line, kWhitespaceASCII, WHITESPACE_TRIM, SPLIT_WANT_NONEMPTY);
        // The HugePages_* entries only have numbers and no suffix. Hence there may not be exactly
        // 3 tokens all the time
        if (tokens.size() <= 1) {
            LOGGING(base::INFO) << "Memory info: tokens: " << tokens.size()
                                << ". Malformed: " << line;
            continue;
        }

        int* target = nullptr;
        // clang-format off
        if (tokens[0] == "MemTotal:")            target = &mem_info->total;
        else if (tokens[0] == "MemFree:")        target = &mem_info->free;
        else if (tokens[0] == "MemAvailable:")   target = &mem_info->available;
        else if (tokens[0] == "Buffers:")        target = &mem_info->buffers;
        else if (tokens[0] == "Cached:")         target = &mem_info->cached;
        else if (tokens[0] == "Active(anon):")   target = &mem_info->active_anon;
        else if (tokens[0] == "Inactive(anon):") target = &mem_info->inactive_anon;
        else if (tokens[0] == "Active(file):")   target = &mem_info->active_file;
        else if (tokens[0] == "Inactive(file):") target = &mem_info->inactive_file;
        else if (tokens[0] == "SwapTotal:")      target = &mem_info->swap_total;
        else if (tokens[0] == "SwapFree:")       target = &mem_info->swap_free;
        else if (tokens[0] == "Dirty:")          target = &mem_info->dirty;
        else if (tokens[0] == "Reclaimable:")    target = &mem_info->reclaimable;
        // clang-format on

        if (target) {
            *target = std::atoi(tokens[1].c_str());
        }
    }

    return mem_info->total > 0;
}
}  // namespace

bool GetSystemMemoryInfo(SystemMemoryInfo* mem_info) {
    file::FilePath mem_info_file("/proc/meminfo");
    std::string mem_info_data;
    if (!file::ReadFile(mem_info_file, mem_info_data)) {
        LOGGING(base::ERROR) << "Failed to open " << mem_info_file.value();
        return false;
    }

    if (!ParseProcMemInfo(mem_info_data, mem_info)) {
        LOGGING(base::ERROR) << "Failed to parse " << mem_info_file.value();
        return false;
    }

    return true;
}

}  // namespace system
}  // namespace kwc
