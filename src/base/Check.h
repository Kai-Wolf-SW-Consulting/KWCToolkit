// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_CHECK_H_
#define KWCTOOLKIT_BASE_CHECK_H_

#include <ostream>

#include "base/Logging.h"  // IWYU pragma: keep

namespace kwc {
namespace base {
namespace internal {

// This gets constructed for every failing KWC_CHECK macro down below.
// CheckHandler will log information about the failure and abort in its destructor
class CheckHandler {
  public:
    CheckHandler(const char* check, const char* file, const char* func, int line);
    ~CheckHandler();
    std::ostream& GetLog();

    CheckHandler& operator=(const CheckHandler&) = delete;
    CheckHandler(const CheckHandler&) = delete;
    CheckHandler() = delete;

  private:
    std::ostream& log_;
};

#ifndef NDEBUG
    #define KWC_CHECK(b)                                       \
        ((b) ? ::kwc::base::internal::GetNullLoggingInstance() \
             : ::kwc::base::internal::CheckHandler(#b, __FILE__, __func__, __LINE__).GetLog())
#else
    #define KWC_CHECK(b) ::kwc::base::internal::GetNullLoggingInstance()
#endif

#define KWC_CHECK_LT(x, y) KWC_CHECK((x) < (y))
#define KWC_CHECK_GT(x, y) KWC_CHECK((x) > (y))
#define KWC_CHECK_LE(x, y) KWC_CHECK((x) <= (y))
#define KWC_CHECK_GE(x, y) KWC_CHECK((x) >= (y))
#define KWC_CHECK_EQ(x, y) KWC_CHECK((x) == (y))
#define KWC_CHECK_NE(x, y) KWC_CHECK((x) != (y))
#define KWC_CHECK_NOTNULL(x) KWC_CHECK((x) != nullptr)

}  // namespace internal
}  // namespace base
}  // namespace kwc

#endif  // KWCTOOLKIT_BASE_CHECK_H_
