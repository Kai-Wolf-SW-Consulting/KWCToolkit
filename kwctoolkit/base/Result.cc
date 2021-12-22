// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/base/Result.h"

namespace kwc {
namespace base {
namespace internal {

intptr_t createPayload(const void* ptr, Payload type) {
    auto payload = reinterpret_cast<intptr_t>(ptr);
    KWC_ASSERT((payload & 3) == 0);
    return payload | type;
}

Payload getPayload(intptr_t payload) {
    return static_cast<Payload>(payload & 3);
}

}  // namespace internal
}  // namespace base
}  // namespace kwc