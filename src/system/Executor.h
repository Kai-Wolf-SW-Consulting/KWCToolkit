// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_SYSTEM_EXECUTOR_H_
#define KWCTOOLKIT_SYSTEM_EXECUTOR_H_

#include "base/Macros.h"

namespace kwc {
class Callback;

namespace system {

// Async execution of callbacks.
//
// The following abstract base class takes callbacks and runs them
// asynchronously. This implementation is loosely based on the original
// proposal N3378 by Google.
//
// There are multiple implementations (so called concrete Executors) possible
// such as |thread_pool| taking new threads out of a thread pool,
// |serial_executor| which processes one task after another or
// |inline_executor|, which is implemented here. The latter calls the callback
// function just right away on the calling thread.
//
// Note that we deliberately choose not to use templates and just pure
// functions as callbacks which don't return anything in favor of a simple
// interface.
class Executor {
  public:
    Executor() = default;
    virtual ~Executor() = default;

    // Schedule the specified callback for execution in this executor.
    virtual void add(Callback* callback) = 0;

    // Caller retains ownership
    static void setDefaultExecutor(Executor* executor);
    static Executor* defaultExecutor();

  private:
    DISALLOW_COPY_AND_ASSIGN(Executor);
};

// Immediately executes callbacks without using threads. The caller should
// delete it afterwards.
Executor* MakeInlineExecutor();

// Ownership is maintained internally by the Executor itself
Executor* SingletonInlineExecutor();

}  // namespace system
}  // namespace kwc

#endif  // KWCTOOLKIT_BASE_EXECUTOR_H_
