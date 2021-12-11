// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include <cstdio>
#include <cstdlib>
#include <ostream>

#include "app/app.h"
#include "base/cmdline_flags.h"
#include "base/logging.h"
#include "base/platform.h"

#if defined(OS_MACOS) || defined(OS_LINUX)
    #include <execinfo.h>

    #include <csignal>

// Setup some debugging info in case the program segfaults hard
inline void SegmentationFaultHandler(int sig) {
    using namespace kwc;
    LOGGING(base::ERROR) << "\n*** *** *** *** *** *** *** *** *** *** *** *** *** ***\n";
    LOGGING(base::ERROR) << "KWCToolkit: " << ::sys_siglist[sig] << "\n";
    LOGGING(base::ERROR) << "Backtrace:\n";
    void* array[50];
    const auto nsize = backtrace(array, 50);
    backtrace_symbols_fd(array, nsize, 2);
    std::exit(-1);
}

void SetupSignalHandlers() {
    signal(SIGSEGV, SegmentationFaultHandler);
}
#else
void SetupSignalHandlers() {}
#endif

namespace kwc {

DEFINE_int32(verbosity, base::INFO, "Log level verbosity");

namespace internal {
void PrintUsageAndExit() {
    fprintf(stdout,
            "main"
            " [--v=<verbosity>]\n");
    std::exit(EXIT_SUCCESS);
}

void ParseCmdLineFlags(int argc, char** argv) {
    using namespace kwc::base;
    for (int idx = 1; idx < argc; ++idx) {
        if (ParseInt32Flag(argv[idx], "v", &FLAGS_verbosity)) {
            for (int jdx = idx; jdx != argc - 1; ++jdx) {
                argv[jdx] = argv[jdx + 1];
            }
            --(argc);
            --idx;
        } else if (IsFlag(argv[idx], "help")) {
            PrintUsageAndExit();
        }
    }
}
}  // namespace internal
}  // namespace kwc

int main(int argc, char* argv[]) {
    using namespace kwc;
    internal::ParseCmdLineFlags(argc, argv);
    base::internal::SetLoggingLevel(FLAGS_verbosity);

    // Setup crash signal handlers
    SetupSignalHandlers();

    LOGGING(base::INFO) << "Hello, KWCToolkit\n";

    return EXIT_SUCCESS;
}