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

// This gets constructed for every failing CHECK macro down below.
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
    #define CHECK(b)                                           \
        ((b) ? ::kwc::base::internal::GetNullLoggingInstance() \
             : ::kwc::base::internal::CheckHandler(#b, __FILE__, __func__, __LINE__).GetLog())
#else
    #define CHECK(b) ::kwc::base::internal::GetNullLoggingInstance()
#endif

#define CHECK_LT(x, y) CHECK((x) < (y))
#define CHECK_GT(x, y) CHECK((x) > (y))
#define CHECK_LE(x, y) CHECK((x) <= (y))
#define CHECK_GE(x, y) CHECK((x) >= (y))
#define CHECK_EQ(x, y) CHECK((x) == (y))
#define CHECK_NE(x, y) CHECK((x) != (y))
#define CHECK_NOTNULL(x) CHECK((x) != nullptr)

}  // namespace internal
}  // namespace base
}  // namespace kwc

#endif  // KWCTOOLKIT_BASE_CHECK_H_
