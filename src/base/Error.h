// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_ERROR_H_
#define KWCTOOLKIT_BASE_ERROR_H_

#include <string>

#include "base/IntegralTypes.h"

namespace kwc {

enum class InternalErrorType : uint32 { OutOfMemory, UnknownFormat, Internal };

#define MAKE_ERROR(TYPE, MESSAGE) \
    base::ErrorTrace::Create(TYPE, MESSAGE, __FILE__, __func__, __LINE__)

#define INTERNAL_ERROR(MESSAGE) MAKE_ERROR(InternalErrorType::Internal, MESSAGE)
#define UNKNOWN_FORMAT_ERROR(MESSAGE) MAKE_ERROR(InternalErrorType::UnknownFormat, MESSAGE)

}  // namespace kwc

#endif  // KWCTOOLKIT_BASE_ERROR_H_
