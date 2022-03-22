// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_STATUS_H_
#define KWCTOOLKIT_BASE_STATUS_H_

#include <string>
#include <utility>

namespace kwc {
namespace base {
namespace error {
enum Code {
    OK = 0,
    CANCELLED = 1,
    UNKNOWN = 2,
    INVALID_ARGUMENT = 3,
    NOT_FOUND = 4,
    DATA_LOSS = 5
};
}  // namespace error

// Determines whether a call or a given object is error free.
// Status objects may be used, whenever one inputs an object into a function
// and wants to check, if the desired transformation could be applied
// successfully.
class Status {
  public:
    Status() = default;
    Status(error::Code code, std::string msg) : code_(code), msg_(std::move(msg)) {}
    Status(const Status& status) = default;
    ~Status() = default;

    Status& operator=(const Status& status) = default;

    bool operator==(const Status& status) const {
        return code_ == status.code_ && msg_ == status.msg_;
    }

    bool ok() const { return code_ == error::OK; }

    const std::string& errorMessage() const { return msg_; }

    error::Code errorCode() const { return code_; }

    std::string toString() const;

  private:
    error::Code code_{error::OK};
    std::string msg_;
};

}  // namespace base
}  // namespace kwc

#endif  // KWCTOOLKIT_BASE_STATUS_H_
