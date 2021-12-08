// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "base/check.h"
#include "base/logging.h"

namespace kwc {
namespace base {
namespace internal {

CheckHandler::CheckHandler(const char* check, const char* file, const char* func, int line)
    : log_(GetErrorLoggingInstance()) {
    log_ << file << ":" << line << ": " << func << ": Check '" << check << "' failed.";
}

CheckHandler::~CheckHandler() {
    log_ << "\n";
}

std::ostream& CheckHandler::GetLog() {
    return log_;
}

}  // namespace internal
}  // namespace base
}  // namespace kwc
