// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "base/assert.h"

#include <cstdlib>
#include <ostream>

#include "base/logging.h"

void handleAssertFailure(const char* file, const char* fn, int line, const char* condition) {
    using namespace kwc::base;
    LOGGING(ERROR) << "Assert failure at " << file << ":" << line << " (" << fn
                   << "): " << condition;
    std::abort();
}