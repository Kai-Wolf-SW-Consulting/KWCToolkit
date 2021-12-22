// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_RW_PROTECTED_H_
#define KWCTOOLKIT_BASE_RW_PROTECTED_H_

#include <cstdint>

namespace kwc {
namespace base {

template <typename t>
class WriteOnly {
  public:
    WriteOnly() = default;
    WriteOnly(const t& val) : val_(val) {}
    // returns void instead of reference to self in order to forbid read access
    void operator=(const t& val) { val_ = val; }
    WriteOnly(const WriteOnly&) = delete;
    WriteOnly& operator=(const WriteOnly&) = delete;

  private:
    t val_;
};

template <typename t>
class ReadOnly {
  public:
    ReadOnly() = default;
    ReadOnly(const t& val) : val_(val) {}
    operator const t&() const { return val_; }
    const t* operator&() const { return &val_; }
    ReadOnly(const ReadOnly&) = delete;
    ReadOnly& operator=(const ReadOnly&) = delete;

  private:
    t val_;
};

}  // namespace base
}  // namespace kwc

#endif  // KWCTOOLKIT_BASE_RW_PROTECTED_H_
