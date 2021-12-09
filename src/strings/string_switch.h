// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_STRINGS_STRING_SWITCH_H_
#define KWCTOOLKIT_STRINGS_STRING_SWITCH_H_

#include <string>
#include <utility>

#include "base/assert.h"
#include "base/macros.h"

namespace kwc {
namespace strings {
namespace {
template <typename T>
struct OptionalResult {
    bool valid{false};
    T value;
};

}  // namespace

// switch-like behaviour but for strings
//
// This class resembles the functionality of a switch() statement that checks if a given string
// does match one of the given string literals.
//
// Note that 'case' and 'default' are keywords in C and C++ and hence cannot be used for naming
// anything else, or functions in particular. Hence, this implementation is using uppercase
// 'Case' and 'Default' to keep the implementation as close as possible to the original behaviour
// of any switch statement.
template <typename T, typename R = T>
class StringSwitch {
  public:
    explicit StringSwitch(const std::string& str) : str_(str), result_() {}

    void operator=(StringSwitch&& other) = delete;

    StringSwitch(StringSwitch&& other) : str_(other.str_), result_(std::move(other.result_)) {}

    ~StringSwitch() = default;

    StringSwitch& Case(const std::string& str, T value) {
        if (!result_.valid && str_ == str) {
            result_.value = std::move(value);
            result_.valid = true;
        }
        return *this;
    }

    R Default(T value) {
        if (result_.valid) {
            return std::move(result_.value);
        }
        return value;
    }

    operator R() {
        ASSERT(result_.valid && "Fell of the end of string switch");
        return std::move(*result_);
    }

  private:
    DISALLOW_COPY_AND_ASSIGN(StringSwitch);
    const std::string str_;
    OptionalResult<T> result_;
};

}  // namespace strings
}  // namespace kwc

#endif  // KWCTOOLKIT_STRINGS_STRING_SWITCH_H_
