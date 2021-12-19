// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_ERROR_TRACE_H_
#define KWCTOOLKIT_BASE_ERROR_TRACE_H_

#include <memory>
#include <string>
#include <vector>

#include "base/Compiler.h"
#include "base/IntegralTypes.h"

namespace kwc {
enum class InternalErrorType : uint32;

namespace base {

class NO_DISCARD ErrorTrace {
  public:
    static std::unique_ptr<ErrorTrace> Create(InternalErrorType type,
                                              const std::string& message,
                                              const char* file,
                                              const char* function,
                                              int line);
    ErrorTrace(InternalErrorType type, const std::string& message);

    struct BacktraceRecord {
        const char* file;
        const char* function;
        int line;
    };
    void AppendBacktrace(const char* file, const char* function, int line);
    NO_DISCARD InternalErrorType GetType() const;
    NO_DISCARD const std::string& GetMessage() const;
    NO_DISCARD const std::vector<BacktraceRecord>& GetBacktrace() const;

  private:
    InternalErrorType type_;
    std::string message_;
    std::vector<BacktraceRecord> backtrace_;
};

}  // namespace base
}  // namespace kwc

#endif  // KWCTOOLKIT_BASE_ERROR_TRACE_H_
