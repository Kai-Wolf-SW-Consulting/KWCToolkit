// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_REF_COUNT_H_
#define KWCTOOLKIT_BASE_REF_COUNT_H_

#include <atomic>
#include <type_traits>

#include "kwctoolkit/base/IntegralTypes.h"

namespace kwc {
namespace base {

// Reference counted pointer implementation
//
// This implementation is used over smart pointers like std::shared_ptr
// for its use of an _intrusive_ count, meaning it can only be used with
// classes and structs that derive from RefCount.
//
// This approach has several advantages: For one, we avoid a separate
// memory allocation to store the count (in the control block), and we
// also get rid of std::make_shared (as well as the overhead associated
// with std::weak_ptr). It's trivial for an object to return a reference
// counted pointer to itself.
//
// In addition to that this implementation returns a _const_ pointer to a
// _const_ object. While this provides no strong guarantees, it very well
// may catch some const mistakes from time to time.
class RefCount {
  public:
    RefCount(uint64 payload = 0);
    virtual ~RefCount();

    uint64 getRefCountForTesting() const;
    uint64 getRefCountPayload() const;

    void reference();
    void release();

  protected:
    std::atomic<uint64> refcount_;
};

template <typename T>
class Ref final {
  public:
    Ref() = default;

    Ref(T* ptr) : pointee_(ptr) { reference(); }

    Ref(const Ref<T>& other) : pointee_(other.pointee_) { reference(); }

    Ref<T>& operator=(const Ref<T>& other) {
        if (&other == this) {
            return *this;
        }

        other.reference();
        release();
        pointee_ = other.pointee_;

        return *this;
    }

    Ref(Ref<T>&& other) noexcept {
        pointee_ = other.pointee_;
        other.pointee_ = nullptr;
    }

    Ref<T>& operator=(Ref<T>&& other) noexcept {
        if (&other == this) {
            return *this;
        }

        release();
        pointee_ = other.pointee_;
        other.pointee_ = nullptr;

        return *this;
    }

    ~Ref() {
        release();
        pointee_ = nullptr;
    }

    operator bool() { return pointee_ != nullptr; }

    const T& operator*() const { return *pointee_; }

    T& operator*() { return pointee_; }

    const T* operator->() const { return pointee_; }

    T* operator->() { return pointee_; }

    const T* get() const { return pointee_; }

    T* get() { return pointee_; }

    T* detach() {
        T* pointee = pointee_;
        pointee_ = nullptr;
        return pointee;
    }

  private:
    void reference() const {
        if (pointee_ != nullptr) {
            pointee_->reference();
        }
    }

    void release() const {
        if (pointee_ != nullptr) {
            pointee_->release();
        }
    }

    static_assert(std::is_base_of<RefCount, T>::value, "");
    T* pointee_ = nullptr;
};

template <typename T>
Ref<T> AcquireRef(T* pointee) {
    Ref<T> ref(pointee);
    ref->release();
    return ref;
}

}  // namespace base
}  // namespace kwc

#endif  // KWCTOOLKIT_BASE_REF_COUNT_H_
