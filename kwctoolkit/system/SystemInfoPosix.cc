// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include <sys/resource.h>
#include <sys/utsname.h>
#include <unistd.h>

#include <string>

#include <kwctoolkit/base/Assert.h>
#include <kwctoolkit/base/IntegralTypes.h>
#include <kwctoolkit/base/Platform.h>
#include <kwctoolkit/system/SystemInfo.h>

namespace kwc {
namespace system {

int SystemInfo::getNumberOfCPUs() {
    // sysconf() actually returns the number of _logical_ (not _physical_) number of cpus on both
    // macOS and Linux.
    //
    // Please note that the number of currently online cpus may be fewer than the returned value of
    // this function due to the kernel making some processors offline intermittently in order to
    // save power when the system load is low.
    //
    // This routine should mainly be used in order to create the _optimal_ number of threads for
    // optimization on some low end devices.
    const auto result = sysconf(_SC_NPROCESSORS_CONF);
    if (result == -1) {
        KWC_UNREACHABLE();
        return 1;
    }

    return static_cast<int>(result);
}

#if !defined(KWC_OS_MACOS) && !defined(KWC_OS_ANDROID)
std::string SystemInfo::getOSName() {
    struct utsname info;
    if (uname(&info) < 0) {
        KWC_UNREACHABLE();
        return std::string();
    }
    return std::string(info.sysname);
}
#endif

#if !defined(KWC_OS_MACOS) && !defined(KWC_OS_ANDROID)
std::string SystemInfo::getOSVersion() {
    struct utsname info;
    if (uname(&info) < 0) {
        KWC_UNREACHABLE();
        return std::string();
    }
    return std::string(info.release);
}
#endif

std::string SystemInfo::getOSArch() {
    struct utsname info;
    if (uname(&info) < 0) {
        KWC_UNREACHABLE();
        return std::string();
    }

    std::string arch(info.machine);
    if (arch == "i386" || arch == "i486" || arch == "i586" || arch == "i686") {
        arch = "x86";
    } else if (arch == "amd64") {
        arch = "x86_64";
    } else if (std::string(info.sysname) == "AIX") {
        arch = "ppc64";
    }
    return arch;
}

int64 SystemInfo::getAmountOfVirtualMemory() {
    struct rlimit limit;
    int result = getrlimit(RLIMIT_DATA, &limit);
    if (result != 0) {
        KWC_UNREACHABLE();
        return 0;
    }
    return limit.rlim_cur == RLIM_INFINITY ? 0 : limit.rlim_cur;
}

}  // namespace system
}  // namespace kwc