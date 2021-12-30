// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/base/ErrorTrace.h"

#include <utility>

namespace kwc {
namespace base {

std::unique_ptr<ErrorTrace> ErrorTrace::create(InternalErrorType type,
                                               const std::string& message,
                                               const char* file,
                                               const char* function,
                                               int line) {
    auto error = std::make_unique<ErrorTrace>(type, message);
    error->appendBacktrace(file, function, line);
    return error;
}

ErrorTrace::ErrorTrace(InternalErrorType type, std::string message)
    : type_(type), message_(std::move(message)) {}

void ErrorTrace::appendBacktrace(const char* file, const char* function, int line) {
    BacktraceRecord record;
    record.file = file;
    record.function = function;
    record.line = line;
    backtrace_.push_back(record);
}

InternalErrorType ErrorTrace::getType() const {
    return type_;
}

const std::string& ErrorTrace::getMessage() const {
    return message_;
}

const std::vector<ErrorTrace::BacktraceRecord>& ErrorTrace::getBacktrace() const {
    return backtrace_;
}

}  // namespace base
}  // namespace kwc
