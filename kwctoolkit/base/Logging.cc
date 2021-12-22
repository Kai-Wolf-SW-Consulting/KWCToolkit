// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/base/Logging.h"

#include <iostream>

namespace kwc {
namespace base {
namespace internal {
inline int& LoggingLevelImpl() {
    static int level = 0;
    return level;
}

void SetLoggingLevel(int value) {
    LoggingLevelImpl() = value;
}

int GetLoggingLevel() {
    return LoggingLevelImpl();
}

class NullLoggingBuffer : public std::streambuf {
  public:
    int overflow(int c) override { return c; }
};

std::ostream& GetNullLoggingInstance() {
    static NullLoggingBuffer log_buffer;
    static std::ostream null_logger(&log_buffer);
    return null_logger;
}

std::ostream& GetErrorLoggingInstance() {
    return std::clog;
}
}  // namespace internal
}  // namespace base
}  // namespace kwc