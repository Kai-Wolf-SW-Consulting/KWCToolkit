// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_SYSTEM_PROCESS_INTERNAL_LINUX_H_
#define KWCTOOLKIT_SYSTEM_PROCESS_INTERNAL_LINUX_H_

#include <unistd.h>

#include <cassert>
#include <cstdlib>
#include <string>
#include <vector>

#include <kwctoolkit/file/FilePath.h>
#include <kwctoolkit/file/FileUtils.h>
#include <kwctoolkit/strings/StringUtils.h>

namespace kwc {
namespace system {

constexpr const char* kProcDir = "/proc";
constexpr const char* kStatFile = "stat";

// Fields from /proc/<pid>/stat. It is defined in /usr/kwctoolkit/linux/fs/proc/array.c.
// See man 5 proc for more info on this.
enum ProcStatsFields {
    PS_COMM = 1,         // Filename of executable
    PS_STATE = 2,        // Letter indicating state of the process
    PS_PPID = 3,         // PID of the parent
    PS_PGRP = 4,         // Process group id
    PS_UTIME = 13,       // Time sched. in user mode (clock ticks)
    PS_STIME = 14,       // Time sched. in kernel mode (clock ticks)
    PS_NUMTHREADS = 19,  // Number of threads
    PS_STARTTIME = 21,   // Time the process started (clock ticks)
    PS_VSIZE = 22,       // Virtual memory size in bytes
    PS_RSS = 23,         // Resident Set Size in pages
};

file::FilePath GetProcPidDirectory(pid_t pid);

pid_t GetProcDirectoryFromPid(const char* d_name);

std::string ReadProcFile(const file::FilePath& file_path);

std::string ReadProcStats(pid_t pid);

std::vector<std::string> ParseProcStats(const std::string& stats_data);

int GetProcStatsFieldAsInt(const std::vector<std::string>& proc_stats, ProcStatsFields field_num);

int ReadProcStatsAndGetFieldAsInt(pid_t pid, ProcStatsFields field_num);

}  // namespace system
}  // namespace kwc

#endif  // KWCTOOLKIT_SYSTEM_PROCESS_INTERNAL_LINUX_H_
