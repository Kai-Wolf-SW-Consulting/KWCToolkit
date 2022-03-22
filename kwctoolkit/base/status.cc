// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/base/status.h"

namespace kwc {
namespace base {

#define CASE(code) \
    case error::code: return #code

static std::string StatusCodeToString(error::Code code) {
    switch (code) {
        CASE(OK);
        default:
            std::string result("Error #");
            result.append(std::to_string(code));
            return result;
    }
}
#undef CASE

std::string Status::toString() const {
    auto result = StatusCodeToString(code_);
    if (!msg_.empty()) {
        result.append(": ");
        result.append(msg_);
    }

    return result;
}
}  // namespace base
}  // namespace kwc
