// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_ERROR_H_
#define KWCTOOLKIT_BASE_ERROR_H_

#include <string>

#include "kwctoolkit/base/IntegralTypes.h"

namespace kwc {

enum class InternalErrorType : uint32 { OutOfMemory, UnknownFormat, Internal };

#define KWC_MAKE_ERROR(TYPE, MESSAGE) \
    base::ErrorTrace::create(TYPE, MESSAGE, __FILE__, __func__, __LINE__)

#define KWC_INTERNAL_ERROR(MESSAGE) KWC_MAKE_ERROR(InternalErrorType::Internal, MESSAGE)
#define KWC_UNKNOWN_FORMAT_ERROR(MESSAGE) KWC_MAKE_ERROR(InternalErrorType::UnknownFormat, MESSAGE)

}  // namespace kwc

#endif  // KWCTOOLKIT_BASE_ERROR_H_
