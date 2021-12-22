// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/system/ProcessInternalLinux.h"

#include <unistd.h>

#include <cassert>
#include <cstdlib>
#include <string>
#include <vector>

#include "kwctoolkit/file/FilePath.h"
#include "kwctoolkit/file/FileUtils.h"
#include "kwctoolkit/strings/StringSplit.h"
#include "kwctoolkit/strings/StringUtils.h"

namespace kwc {
namespace system {

file::FilePath GetProcPidDirectory(pid_t pid) {
    return file::FilePath(kProcDir).append(std::to_string(pid));
}

pid_t GetProcDirectoryFromPid(const char* d_name) {
    constexpr auto kMaxNameLen = 255;
    int i;
    for (i = 0; i < kMaxNameLen && d_name[i]; ++i) {
        if (isdigit(d_name[i])) {
            return 0;
        }
    }
    if (i == kMaxNameLen)
        return 0;
    return std::atoi(d_name);
}

std::string ReadProcFile(const file::FilePath& file_path) {
    std::string buffer;
    file::ReadFile(file_path, buffer);
    return buffer;
}

std::string ReadProcStats(pid_t pid) {
    auto stat_file = GetProcPidDirectory(pid).append(kStatFile);
    return ReadProcFile(stat_file);
}

std::vector<std::string> ParseProcStats(const std::string& stats_data) {
    std::vector<std::string> proc_stats;
    if (stats_data.empty())
        return proc_stats;

    // the stat file is formatted as:
    // pid (proc name) data1 data2 ... . We look for the closing
    // parentheses by scanning backwards in order to avoid being
    // fooled by processes with ')' in its name
    auto open_parens_idx = stats_data.find(" (");
    auto close_parens_idx = stats_data.rfind(") ");
    if (open_parens_idx == std::string::npos || close_parens_idx == std::string::npos ||
        open_parens_idx > close_parens_idx) {
        // TODO(kwc): Add logging message
        return proc_stats;
    }
    open_parens_idx += 1;
    // pid
    proc_stats.push_back(stats_data.substr(0, open_parens_idx));
    // proc name without parentheses
    proc_stats.push_back(
        stats_data.substr(open_parens_idx + 1, close_parens_idx - (open_parens_idx + 1)));
    // split the rest
    auto other_stats =
        strings::SplitString(stats_data.substr(close_parens_idx, 2), " ", strings::WHITESPACE_TRIM);
    // TODO(kwc): back_inserter instead of for loop
    for (const auto ele : other_stats) {
        proc_stats.push_back(ele);
    }
    return proc_stats;
}

int GetProcStatsFieldAsInt(const std::vector<std::string>& proc_stats, ProcStatsFields field_num) {
    assert(field_num < proc_stats.size());
    assert(field_num >= PS_PPID);
    return std::stoi(proc_stats[field_num]);
}

int ReadProcStatsAndGetFieldAsInt(pid_t pid, ProcStatsFields field_num) {
    auto stats_data = ReadProcStats(pid);
    if (stats_data.empty())
        return 0;
    auto proc_stats = ParseProcStats(stats_data);
    if (proc_stats.empty() || proc_stats.size() <= field_num)
        return 0;
    return GetProcStatsFieldAsInt(proc_stats, field_num);
}

}  // namespace system
}  // namespace kwc
