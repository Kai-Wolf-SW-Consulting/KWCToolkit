// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "system/Executor.h"

#include <mutex>

#include "base/Callback.h"

namespace kwc {
namespace {
using system::Executor;

Executor* default_executor = nullptr;
Executor* global_inline_executor = nullptr;

std::once_flag module_init;
std::mutex g_mutex;

// Simple executor without queueing.
//
// This class is a simple executor which intrinsically only provides the
// |add()| interface, as it provides no queueing and instead immediately
// executes work on the calling thread. This is effectively an adapter over
// the executor interface, but keeps everything on the caller's context.
class InlineExecutor : public Executor {
  public:
    ~InlineExecutor() override = default;

    void add(Callback* callback) override { callback->run(); }
};

void initModule() {
    global_inline_executor = new InlineExecutor;
    default_executor = global_inline_executor;
}
}  // namespace

namespace system {

Executor* Executor::defaultExecutor() {
    std::call_once(module_init, initModule);
    return default_executor;
}

void Executor::setDefaultExecutor(Executor* executor) {
    std::call_once(module_init, initModule);
    std::lock_guard<std::mutex> guard(g_mutex);
    default_executor = executor;
}

Executor* MakeInlineExecutor() {
    return new InlineExecutor;
}

Executor* SingletonInlineExecutor() {
    std::call_once(module_init, initModule);
    return global_inline_executor;
}

}  // namespace system
}  // namespace kwc
