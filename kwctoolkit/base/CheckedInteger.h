// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_CHECKED_INTEGER_H_
#define KWCTOOLKIT_BASE_CHECKED_INTEGER_H_

#include <limits>
#include <type_traits>

#include "kwctoolkit/base/Assert.h"

namespace kwc {
namespace base {
namespace internal {
template <typename Tag, typename T>
class CheckedIntegerImpl;
}  // namespace internal

template <bool Condition, typename T = void>
using enable_if_t = typename std::enable_if<Condition, T>::type;

// The typedef CheckedInteger is a utility that provides additional type safety for integer
// overflow and other errors in Debug mode.
//
// This implementation helps to make sure that index and integral types do not get mixed up
// and end up being used interchangeably. For Release builds CheckedInteger simply does a
// passthrough of the underlying type.
// For instance:
//
//     using UIntA = CheckedInteger<struct TypeA, uint32>;
//     using UIntB = CheckedInteger<struct TypeB, uint32>;
//
// Debug build:
//     using UIntA = kwc::base::internal::CheckedIntegerImpl<struct TypeA, uint32>;
//     using UIntB = kwc::base::internal::CheckedIntegerImpl<struct TypeB, uint32>;
//
// Release build:
//     using UIntA = uint32;
//     using UIntB = uint32;
//
// All operations on integral types such as assignment, construction, comparison and arithmetic
// are only allowed for typed integers of exactly the same type.
//
template <typename Tag, typename T, typename = enable_if_t<std::is_integral<T>::value>>
#if BUILD_FLAG(ENABLE_ASSERTS)
using CheckedInteger = internal::CheckedIntegerImpl<Tag, T>;
#else
using CheckedInteger = T;
#endif

namespace internal {
template <typename Tag, typename T>
class alignas(T) CheckedIntegerImpl {
    static_assert(std::is_integral<T>::value, "CheckedInteger must be integral");
    T value_;

  public:
    constexpr CheckedIntegerImpl() : value_(0) {
        static_assert(alignof(CheckedIntegerImpl) == alignof(T), "");
        static_assert(sizeof(CheckedIntegerImpl) == sizeof(T), "");
    }

    // Construction from non-narrowing integral types
    template <
        typename I,
        typename = enable_if_t<std::is_integral<I>::value &&
                               std::numeric_limits<I>::max() <= std::numeric_limits<T>::max() &&
                               std::numeric_limits<I>::min() >= std::numeric_limits<T>::min()>>
    explicit constexpr CheckedIntegerImpl(I rhs) : value_(static_cast<T>(rhs)) {}

    // Explicit cast to the underlying type
    explicit constexpr operator T() const { return static_cast<T>(this->value_); }

#define CHECKED_COMPARISON(op) \
    constexpr bool operator op(const CheckedIntegerImpl& rhs) const { return value_ op rhs.value_; }
    CHECKED_COMPARISON(<)
    CHECKED_COMPARISON(<=)
    CHECKED_COMPARISON(>)
    CHECKED_COMPARISON(>=)
    CHECKED_COMPARISON(==)
    CHECKED_COMPARISON(!=)
#undef CHECKED_COMPARISON

    // Increment / decrement operators for for-loop iteration
    constexpr CheckedIntegerImpl& operator++() {
        KWC_ASSERT(this->value_ < std::numeric_limits<T>::max());
        ++this->value_;
        return *this;
    }

    constexpr CheckedIntegerImpl operator++(int) {
        CheckedIntegerImpl ret = *this;

        KWC_ASSERT(this->value_ < std::numeric_limits<T>::max());
        ++this->value_;
        return ret;
    }

    constexpr CheckedIntegerImpl& operator--() {
        KWC_ASSERT(this->value_ > std::numeric_limits<T>::min());
        --this->value_;
        return *this;
    }

    constexpr CheckedIntegerImpl operator--(int) {
        CheckedIntegerImpl ret = *this;

        KWC_ASSERT(this->value_ > std::numeric_limits<T>::min());
        --this->value_;
        return ret;
    }

    template <typename T2 = T>
    constexpr enable_if_t<std::is_signed<T2>::value, CheckedIntegerImpl> operator-() const {
        static_assert(std::is_same<T, T2>::value, "");
        // The negation of the most negative value cannot be represented.
        KWC_ASSERT(this->value_ != std::numeric_limits<T>::min());
        return CheckedIntegerImpl(-this->value_);
    }

    template <typename T2 = T>
    constexpr enable_if_t<std::is_unsigned<T2>::value, CheckedIntegerImpl> operator+(
        CheckedIntegerImpl rhs) const {
        static_assert(std::is_same<T, T2>::value, "");
        // Overflow would wrap around
        KWC_ASSERT(this->value_ + rhs.value_ >= this->value_);

        return CheckedIntegerImpl(this->value_ + rhs.value_);
    }

    template <typename T2 = T>
    constexpr enable_if_t<std::is_unsigned<T2>::value, CheckedIntegerImpl> operator-(
        CheckedIntegerImpl rhs) const {
        static_assert(std::is_same<T, T2>::value, "");
        // Overflow would wrap around
        KWC_ASSERT(this->value_ - rhs.value_ <= this->value_);
        return CheckedIntegerImpl(this->value_ - rhs.value_);
    }

    template <typename T2 = T>
    constexpr enable_if_t<std::is_signed<T2>::value, CheckedIntegerImpl> operator+(
        CheckedIntegerImpl rhs) const {
        static_assert(std::is_same<T, T2>::value, "");
        if (this->value_ > 0) {
            KWC_ASSERT(rhs.value_ <= std::numeric_limits<T>::max() - this->value_);
        } else {
            KWC_ASSERT(rhs.value_ >= std::numeric_limits<T>::min() - this->value_);
        }
        return CheckedIntegerImpl(this->value_ + rhs.value_);
    }

    template <typename T2 = T>
    constexpr enable_if_t<std::is_signed<T2>::value, CheckedIntegerImpl> operator-(
        CheckedIntegerImpl rhs) const {
        static_assert(std::is_same<T, T2>::value, "");
        if (this->value_ > 0) {
            KWC_ASSERT(rhs.value_ >= this->value_ - std::numeric_limits<T>::max());
        } else {
            KWC_ASSERT(rhs.value_ <= this->value_ - std::numeric_limits<T>::min());
        }
        return CheckedIntegerImpl(this->value_ - rhs.value_);
    }
};

}  // namespace internal
}  // namespace base
}  // namespace kwc

namespace std {
template <typename Tag, typename T>
class numeric_limits<kwc::base::internal::CheckedIntegerImpl<Tag, T>> : public numeric_limits<T> {
  public:
    static kwc::base::internal::CheckedIntegerImpl<Tag, T> max() noexcept {
        return kwc::base::internal::CheckedIntegerImpl<Tag, T>(std::numeric_limits<T>::max());
    }
    static kwc::base::internal::CheckedIntegerImpl<Tag, T> min() noexcept {
        return kwc::base::internal::CheckedIntegerImpl<Tag, T>(std::numeric_limits<T>::min());
    }
};
}  // namespace std

#endif  // KWCTOOLKIT_BASE_CHECKEDINTEGER_H_
