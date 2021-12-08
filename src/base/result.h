// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_RESULT_H_
#define KWCTOOLKIT_BASE_RESULT_H_

#include <cstdint>
#include <memory>
#include <type_traits>
#include <utility>  // IWYU pragma: keep

#include "base/assert.h"
#include "base/ref_count.h"

namespace kwc {
namespace base {

// The Result type in this class represents the success or failure of an operation, together
// with either the success value or the error itself.
//
// There exist many (public) functions that do acquire some sort of resource and need to
// do error checking at the same time. This mandates that the user of such a function
//
//     1. Calls the function
//     2. Checks the status of the previous action (typically using Status)
//     3. Do the (optional) error handling or
//     4. Use the acquired resource
//
// This is cumbersome from a usability perspective and can be avoided by introducing another type
// in the form of the following class Result<T, E> which follows loosely the notion from Haskell's
// sum type or Rust's result type where:
//
//     data Result T E = Success T | Error E | Empty
//
// This type is meant to be used as a return type for all functions that may fail such that Result
// holds either the required resource (success case), an error object (error case) or an empty
// case which exists solely for the reason that a Result should/can never be discarded in order to
// enforce that error handling has to be done.
//
// Since C++ lacks of a proper sum/result type, this implementation provides templates, or rather
// template specializations for the following types:
//
//     class Result<void, E>
//     class Result<T*, E>
//     class Result<const T*, E>
//     class Result<Ref<T>, E>
//     class Result<T, E> // Fallback definition catching everything else

// The common interface of the Result<T, E> type
template <typename T, typename E>
class Result;

// Template specialization for returning errors via pointers
template <typename E>
class Result<void, E> {
  public:
    Result() = default;

    Result(std::unique_ptr<E> error) : error_(std::move(error)) {}

    Result(Result<void, E>&& other) : error_(std::move(other.error_)) {}

    Result<void, E>& operator=(Result<void, E>&& other) {
        ASSERT(error_ == nullptr);
        error_ = std::move(other.error_);
        return *this;
    }

    ~Result() { ASSERT(error_ == nullptr); }

    bool isError() const { return error_ != nullptr; }

    bool isSuccess() const { return error_ == nullptr; }

    void getSuccess() {}

    std::unique_ptr<E> getError() { return std::move(error_); }

  private:
    std::unique_ptr<E> error_;
};

// Template specialization when both the error and success are pointers
// This is implemented as a tagged pointer, where success is 0, so that returning the value
// is taking the fast path
namespace internal {
// Utility functions for manipulating the tagged pointer, indicating success or error
enum Payload {
    Success = 0,
    Error = 1,
    Empty = 2,
};

intptr_t createPayload(const void* ptr, Payload type);
Payload getPayload(intptr_t payload);

template <typename T>
static T* getSuccessFromPayload(intptr_t payload) {
    ASSERT(getPayload(payload) == Success);
    return reinterpret_cast<T*>(payload);
}

template <typename E>
static E* getErrorFromPayload(intptr_t payload) {
    ASSERT(getPayload(payload) == Error);
    return reinterpret_cast<E*>(payload ^ 1);
}

constexpr static intptr_t kEmptyPayload = Empty;

}  // namespace internal

template <typename T, typename E>
class Result<T*, E> {
  public:
    static_assert(std::alignment_of<T>::value >= 4,
                  "Result<T*, E*> needs two bits for tagged pointers");
    static_assert(std::alignment_of<E>::value >= 4,
                  "Result<T*, E*> needs two bits for tagged pointers");

    Result(T* success) : payload_(internal::createPayload(success, internal::Success)) {}

    Result(std::unique_ptr<E> error)
        : payload_(internal::createPayload(error.release(), internal::Error)) {}

    /** Return a Result<T*, E*> from a Result<TChild*, E*> */
    template <typename TChild>
    Result(Result<TChild*, E>&& other) : payload_(other.payload_) {
        other.payload_ = internal::kEmptyPayload;
        static_assert(std::is_same<T, TChild>::value || std::is_base_of<T, TChild>::value, "");
    }

    template <typename TChild>
    Result<T*, E>& operator=(Result<TChild*, E>&& other) {
        ASSERT(payload_ == internal::kEmptyPayload);
        static_assert(std::is_same<T, TChild>::value || std::is_base_of<T, TChild>::value, "");
        payload_ = other.payload_;
        other.payload_ = internal::kEmptyPayload;
        return *this;
    }

    ~Result() { ASSERT(payload_ == internal::kEmptyPayload); }

    bool isError() const { return internal::getPayload(payload_) == internal::Error; }

    bool isSuccess() const { return internal::getPayload(payload_) == internal::Success; }

    T* getSuccess() {
        T* success = internal::getSuccessFromPayload<T>(payload_);
        payload_ = internal::kEmptyPayload;
        return success;
    }

    std::unique_ptr<E> getError() {
        std::unique_ptr<E> error(internal::getErrorFromPayload<E>(payload_));
        payload_ = internal::kEmptyPayload;
        return std::move(error);
    }

  private:
    template <typename T2, typename E2>
    friend class Result;

    intptr_t payload_ = internal::kEmptyPayload;
};

template <typename T, typename E>
class Result<const T*, E> {
  public:
    static_assert(std::alignment_of<T>::value >= 4,
                  "Result<T*, E*> needs two bits for tagged pointers");
    static_assert(std::alignment_of<E>::value >= 4,
                  "Result<T*, E*> needs two bits for tagged pointers");

    Result(const T* success) : payload_(internal::createPayload(success, internal::Success)) {}

    Result(std::unique_ptr<E> error)
        : payload_(internal::createPayload(error.release(), internal::Error)) {}

    Result(Result<const T*, E>&& other) : payload_(other.payload_) {
        other.payload_ = internal::kEmptyPayload;
    }

    Result<const T*, E>& operator=(Result<const T*, E>&& other) {
        ASSERT(payload_ == internal::kEmptyPayload);
        payload_ = other.payload_;
        other.payload_ = internal::kEmptyPayload;
        return *this;
    }

    ~Result() { ASSERT(payload_ == internal::kEmptyPayload); }

    bool isError() const { return internal::getPayload(payload_) == internal::Error; }

    bool isSuccess() const { return internal::getPayload(payload_) == internal::Success; }

    const T* getSuccess() {
        T* success = internal::getSuccessFromPayload<T>(payload_);
        payload_ = internal::kEmptyPayload;
        return success;
    }

    std::unique_ptr<E> getError() {
        std::unique_ptr<E> error(internal::getErrorFromPayload<E>(payload_));
        payload_ = internal::kEmptyPayload;
        return std::move(error);
    }

  private:
    intptr_t payload_ = internal::kEmptyPayload;
};

template <typename T, typename E>
class Result<Ref<T>, E> {
  public:
    static_assert(std::alignment_of<T>::value >= 4,
                  "Result<T*, E*> needs two bits for tagged pointers");
    static_assert(std::alignment_of<E>::value >= 4,
                  "Result<T*, E*> needs two bits for tagged pointers");

    template <typename U>
    Result(Ref<U>&& success)
        : payload_(internal::createPayload(success.detach(), internal::Success)) {
        static_assert(std::is_convertible<U*, T*>::value, "");
    }

    Result(std::unique_ptr<E> error)
        : payload_(internal::createPayload(error.release(), internal::Error)) {}

    template <typename U>
    Result(Result<Ref<U>, E>&& other) : payload_(other.payload_) {
        static_assert(std::is_convertible<U*, T*>::value, "");
        other.payload_ = internal::kEmptyPayload;
    }

    template <typename U>
    Result<Ref<U>, E>& operator=(Result<Ref<U>, E>&& other) {
        static_assert(std::is_convertible<U*, T*>::value, "");
        ASSERT(payload_ == internal::kEmptyPayload);
        payload_ = other.payload_;
        other.payload_ = internal::kEmptyPayload;
        return *this;
    }

    ~Result() { ASSERT(payload_ == internal::kEmptyPayload); }

    bool isError() const { return internal::getPayload(payload_) == internal::Error; }

    bool isSuccess() const { return internal::getPayload(payload_) == internal::Success; }

    Ref<T> getSuccess() {
        ASSERT(isSuccess());
        auto success = acquireRef(internal::getSuccessFromPayload<T>(payload_));
        payload_ = internal::kEmptyPayload;
        return success;
    }

    std::unique_ptr<E> getError() {
        ASSERT(isError());
        std::unique_ptr<E> error(internal::getErrorFromPayload<E>(payload_));
        payload_ = internal::kEmptyPayload;
        return std::move(error);
    }

  private:
    template <typename T2, typename E2>
    friend class Result;

    intptr_t payload_ = internal::kEmptyPayload;
};

template <typename T, typename E>
class Result {
  public:
    Result(T&& success) : type_(Success), success_(std::move(success)) {}

    Result(std::unique_ptr<E> error) : type_(Error), error_(std::move(error)) {}

    Result(Result<T, E>&& other)
        : type_(other.type_), error_(std::move(other.error_)), success_(std::move(other.success_)) {
        other.type_ = Received;
    }

    Result<T, E>& operator=(Result<T, E>&& other) {
        type_ = other.type_;
        error_ = std::move(other.error_);
        success_ = std::move(other.success_);
        other.type_ = Received;
        return *this;
    }

    ~Result() { ASSERT(type_ == Received); }

    bool isError() const { return type_ == Error; }

    bool isSuccess() const { return type_ == Success; }

    T&& getSuccess() {
        ASSERT(type_ == Success);
        type_ = Received;
        return std::move(success_);
    }

    std::unique_ptr<E> getError() {
        ASSERT(type_ = Error);
        type_ = Received;
        return std::move(error_);
    }

  private:
    enum Payload {
        Success = 0,
        Error = 1,
        Received = 2,
    };

    Payload type_;
    std::unique_ptr<E> error_;
    T success_;
};

}  // namespace base
}  // namespace kwc

#endif  // KWCTOOLKIT_BASE_RESULT_H_
