// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_NET_ADDRESSTYPES_H_
#define KWCTOOLKIT_NET_ADDRESSTYPES_H_

#include "kwctoolkit/base/ErrorTrace.h"
#include "kwctoolkit/base/Result.h"

namespace kwc {
namespace net {

template <typename T>
using AddressOrError = base::Result<T, base::ErrorTrace>;

}
}  // namespace kwc

#endif  // KWCTOOLKIT_KWCTOOLKIT_NET_ADDRESSTYPES_H_
