// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/base/RefCount.h"

#include <cstddef>

#include <kwctoolkit/base/Assert.h>

namespace kwc {
namespace base {
// Please note that RefCount is designed to move in steps of two at a
// time in order to be able to inline a 1 bit (boolean) payload into the
// refcount. This avoids some nasty padding for inherited classes that use
// a (boolean) error variable
static constexpr size_t kPayloadBits = 1;
static constexpr uint64 kPayloadMask = (uint64(1) << kPayloadBits) - 1;
static constexpr uint64 kRefCountIncrement = (uint64(1) << kPayloadBits);

RefCount::RefCount(uint64 payload) : refcount_(kRefCountIncrement + payload) {
    KWC_ASSERT((payload & kPayloadMask) == payload);
}

RefCount::~RefCount() = default;

uint64 RefCount::getRefCountForTesting() const {
    return refcount_ >> kPayloadBits;
}

uint64 RefCount::getRefCountPayload() const {
    // The payload bits of the refcount never change after initialization.
    // Hence, we can use the relaxed memory order here, which does not
    // guarantee anything except the atomicity of the load which is as well
    // sufficient here.
    return kPayloadMask & refcount_.load(std::memory_order_relaxed);
}

void RefCount::reference() {
    KWC_ASSERT((refcount_ & ~kPayloadMask) != 0);

    // See explanation above for why relaxed memory order is sufficient here.
    // The reasoning behind this is also explained in the Boost
    // documentation:
    //   https://www.boost.org/doc/libs/1_72_0/doc/html/atomic/usage_examples.html
    refcount_.fetch_add(kRefCountIncrement, std::memory_order_relaxed);
}

void RefCount::release() {
    KWC_ASSERT((refcount_ & kPayloadMask) != 0);

    // The reasoning behind the following is again very well explained in
    // the Boost documentation referenced above.
    //
    // We basically need to ensure that all accesses on thread 1 happens
    // before the reference count is decreased and the atomic variable makes
    // sure that the reference count does get decreased in thread 1 before
    // the reference count decreases in a (potential) thread 2.
    //
    // Finally, the acquire fence in the destruction case makes sure that
    // the reference count decrease does happen in thread 2 before the last
    // *delete this*.
    const auto prevRefCount = refcount_.fetch_sub(kRefCountIncrement, std::memory_order_release);

    if (prevRefCount < 2 * kRefCountIncrement) {
        std::atomic_thread_fence(std::memory_order_acquire);
        delete this;
    }
}

}  // namespace base
}  // namespace kwc