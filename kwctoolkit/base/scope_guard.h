// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_SCOPE_GUARD_H_
#define KWCTOOLKIT_BASE_SCOPE_GUARD_H_

#include <utility>

#include "kwctoolkit/base/macros.h"

namespace kwc {
namespace base {

// This idiom ensures that resources always get released in face of an
// exception or otherwise not returning normally. |ScopeGuard| can exploit
// type inference and auto, move semantics as well as lambda functions which
// can defer any arbitrary code_
template <typename Func>
class ScopeGuard {
  public:
    ScopeGuard() = delete;
    ScopeGuard(const ScopeGuard&) = delete;
    ScopeGuard& operator=(const ScopeGuard&) = delete;
    ScopeGuard(ScopeGuard&& rhs) noexcept : func_(std::move(rhs.func_)), isActive_(rhs.isActive_) {
        rhs.dismiss();
    }
    ScopeGuard(Func func) : func_(std::move(func)), isActive_(true) {}
    ~ScopeGuard() {
        if (isActive_)
            func_();
    }
    void dismiss() { isActive_ = false; }

  private:
    Func func_;
    bool isActive_;
};

// This is a classic trick, having a template class, which we want to
// associate with a template function, because a template function may use
// type deduction, which the template class is incapable of.
// Example usage:
//     void fun() {
//         auto buf = malloc(1024 * 1024);
//         auto g1 = scopeGuard([] { free(buf); });
//         // .. use buf here ..
//     }
template <typename Func>
ScopeGuard<Func> scopeGuard(Func func) {  // NOLINT
    return ScopeGuard<Func>(std::move(func));
}

namespace internal {
enum class ScopeGuardOnExit {};
template <typename Func>
ScopeGuard<Func> operator+(ScopeGuardOnExit, Func&& fn) {
    return ScopeGuard<Func>(std::forward<Func>(fn));
}
}  // namespace internal
}  // namespace base
}  // namespace kwc

// This works as follows: |ANONYMOUS_VARIABLE| creates a variable which
// shouldn't clash with any other variable in the current scope. This variable
// gets initialized with |ScopeGuardOnExit| plus an unfinished lambda
// function. Note that we're not interested in |ScopeGuardOnExit| itself, but
// only need its type. If |KWC_SCOPE_EXIT| is used, the user finishes it with {}
// and inserts the code_, which gets executed when the current scope is left.
// Example usage:
//     void fun() {
//         char name[] = "/tmp/deleteme.XXXXXX";
//         auto fd = mkstemp(name);
//         KWC_SCOPE_EXIT { fclose(fd); unlink(name); };
//         // .. use fd here ..
//     }
#define KWC_SCOPE_EXIT \
    auto ANONYMOUS_VARIABLE(KWC_SCOPE_EXIT_STATE) = kwc::base::internal::ScopeGuardOnExit() + [&]()

#endif  // KWCTOOLKIT_BASE_SCOPE_GUARD_H_
