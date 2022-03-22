// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_ERROR_TRACE_H_
#define KWCTOOLKIT_BASE_ERROR_TRACE_H_

#include <memory>
#include <string>
#include <vector>

#include "kwctoolkit/base/compiler.h"
#include "kwctoolkit/base/integral_types.h"

namespace kwc {
enum class InternalErrorType : uint32;

namespace base {

class KWC_NO_DISCARD ErrorTrace {
  public:
    static std::unique_ptr<ErrorTrace> create(InternalErrorType type,
                                              const std::string& message,
                                              const char* file,
                                              const char* function,
                                              int line);
    ErrorTrace(InternalErrorType type, std::string message);

    struct BacktraceRecord {
        const char* file;
        const char* function;
        int line;
    };
    void appendBacktrace(const char* file, const char* function, int line);
    KWC_NO_DISCARD InternalErrorType getType() const;
    KWC_NO_DISCARD const std::string& getMessage() const;
    KWC_NO_DISCARD const std::vector<BacktraceRecord>& getBacktrace() const;

  private:
    InternalErrorType type_;
    std::string message_;
    std::vector<BacktraceRecord> backtrace_;
};

}  // namespace base
}  // namespace kwc

#endif  // KWCTOOLKIT_BASE_ERROR_TRACE_H_
