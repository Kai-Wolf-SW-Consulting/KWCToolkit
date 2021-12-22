// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_LOGGING_H_
#define KWCTOOLKIT_BASE_LOGGING_H_

#include <ostream>

#include "kwctoolkit/base/Platform.h"  // IWYU pragma: keep

#if defined(KWC_OS_WINDOWS)
    #undef ERROR
#endif

namespace kwc {
namespace base {

enum LogSeverity { FATAL, ERROR, WARNING, INFO };

namespace internal {
int GetLoggingLevel();
void SetLoggingLevel(int value);

std::ostream& GetNullLoggingInstance();
std::ostream& GetErrorLoggingInstance();

inline std::ostream& GetLoggingInstanceForLevel(int level) {
    if (level <= GetLoggingLevel()) {
        return GetErrorLoggingInstance();
    }
    return GetNullLoggingInstance();
}
}  // namespace internal
}  // namespace base

#define LOGGING(x) \
    (::kwc::base::internal::GetLoggingInstanceForLevel(x) << "-- KWCToolkit LOG(" << (x) << "): ")

}  // namespace kwc

#endif  // KWCTOOLKIT_BASE_LOGGING_H_
