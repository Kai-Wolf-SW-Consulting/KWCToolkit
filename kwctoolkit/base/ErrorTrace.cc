// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/base/ErrorTrace.h"

#include <utility>

namespace kwc {
namespace base {

std::unique_ptr<ErrorTrace> ErrorTrace::Create(InternalErrorType type,
                                               const std::string& message,
                                               const char* file,
                                               const char* function,
                                               int line) {
    auto error = std::make_unique<ErrorTrace>(type, message);
    error->AppendBacktrace(file, function, line);
    return error;
}

ErrorTrace::ErrorTrace(InternalErrorType type, const std::string& message)
    : type_(type), message_(message) {}

void ErrorTrace::AppendBacktrace(const char* file, const char* function, int line) {
    BacktraceRecord record;
    record.file = file;
    record.function = function;
    record.line = line;
    backtrace_.push_back(std::move(record));
}

InternalErrorType ErrorTrace::GetType() const {
    return type_;
}

const std::string& ErrorTrace::GetMessage() const {
    return message_;
}

const std::vector<ErrorTrace::BacktraceRecord>& ErrorTrace::GetBacktrace() const {
    return backtrace_;
}

}  // namespace base
}  // namespace kwc
