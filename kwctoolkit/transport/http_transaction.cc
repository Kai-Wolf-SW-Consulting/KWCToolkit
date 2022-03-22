// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/transport/http_transaction.h"

#include "kwctoolkit/system/executor.h"

namespace kwc {
namespace transport {

HttpTransactionOptions::HttpTransactionOptions() {
    user_agent_ = "Chair";
}

system::Executor* HttpTransactionOptions::getExecutor() const {
    if (executor_ == nullptr) {
        return system::Executor::defaultExecutor();
    }

    return executor_;
}

system::Executor* HttpTransactionOptions::getCallbackExecutor() const {
    if (callback_executor_ == nullptr) {
        return system::SingletonInlineExecutor();
    }

    return callback_executor_;
}
void HttpTransactionOptions::setConnectionTimeout(int64 timeout_ms) {
    connection_timeout_ms_ = timeout_ms;
}

HttpTransaction::HttpTransaction(const HttpTransactionOptions& options) : options_(options) {}

HttpTransactionFactory::HttpTransactionFactory() : default_id_("unknown") {}

std::string HttpTransactionFactory::getDefaultId() const {
    return default_id_;
}

void HttpTransactionFactory::setDefaultId(const std::string& id) {
    default_id_ = id;
}

std::unique_ptr<HttpTransaction> HttpTransactionFactory::createTransaction(
    const HttpTransactionOptions& options) {
    return alloc(options);
}
}  // namespace transport
}  // namespace kwc