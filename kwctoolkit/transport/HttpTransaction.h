// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_TRANSPORT_HTTP_TRANSACTION_H_
#define KWCTOOLKIT_TRANSPORT_HTTP_TRANSACTION_H_

#include <memory>
#include <string>

#include "kwctoolkit/base/IntegralTypes.h"
#include "kwctoolkit/base/Macros.h"
#include "kwctoolkit/transport/HttpRequest.h"
#include "kwctoolkit/transport/HttpTypes.h"

namespace kwc {
namespace system {
class Executor;
}  // namespace system

namespace transport {
// Configures options on an HttpTransaction instance.
// Each HttpTransaction instance maintains its own options. However typically
// the default options are set on the HttpTransactionFactory so that the
// options will apply to all transactions.
class HttpTransactionOptions {
  public:
    HttpTransactionOptions();
    virtual ~HttpTransactionOptions() = default;

    void setExecutor(system::Executor* get_executor) { executor_ = get_executor; }

    // Returns the getExecutor that should be used with this transaction
    system::Executor* getExecutor() const;

    // Set the get_executor to use for the HTTP callback responses
    void setCallbackExecutor(system::Executor* get_executor) { callback_executor_ = get_executor; }

    system::Executor* getCallbackExecutor() const;

    const std::string getUserAgent() const { return user_agent_; }

    void setServer(const std::string& host, int port) {
        host_ = host;
        port_ = port;
    }

    unsigned short port() const { return port_; }

    const std::string host() const { return host_; }

    void setUserAgent(const std::string& agent) { user_agent_ = agent; }

    int64 connectionTimeout() const { return connection_timeout_ms_; }

    void setConnectionTimeout(int64 timeout_ms);

  private:
    unsigned short port_{0};
    std::string host_;
    std::string user_agent_;
    int64 connection_timeout_ms_{0L};
    system::Executor* executor_{nullptr};
    system::Executor* callback_executor_{nullptr};
};

// The following class defines an abstract interface for creating concrete
// HttpTransaction instances.
//
// This class implements a factory pattern for instantiating new
// HttpTransaction instances. It acts as a base class for all HttpTransaction
// factories. It is recommended that one always uses this class when defining
// interfaces rather than the concrete subclasses.
//
// The current HttpTransaction::defaultTransactionFactory must be explicitly
// set at some point during the execution of the application.
class HttpTransactionFactory {
  public:
    HttpTransactionFactory();
    virtual ~HttpTransactionFactory() = default;
    std::unique_ptr<HttpTransaction> createTransaction(const HttpTransactionOptions& options);
    std::string getDefaultId() const;

  protected:
    void setDefaultId(const std::string& id);
    virtual std::unique_ptr<HttpTransaction> alloc(const HttpTransactionOptions& options) = 0;

  private:
    std::string default_id_;
};

// Defines an abstract interface for an HTTP transaction.
// The following class defines an abstract interface for an Http transaction
// which will be specialized for different concrete mechanisms for interacting
// with HTTP servers.
class HttpTransaction {
  public:
    explicit HttpTransaction(const HttpTransactionOptions& options);

    virtual ~HttpTransaction() = default;

    const std::string getUserAgent() const { return getOptions().getUserAgent(); }

    void setId(const std::string& id) { id_ = id; }

    std::string getId() const { return id_; }

    const HttpTransactionOptions getOptions() const { return options_; }

    const HttpRequestOptions getRequestOptions() const { return request_options_; }

    virtual std::unique_ptr<HttpRequest> createHttpRequest(
        const HttpRequest::HttpMethod& method) = 0;

  private:
    std::string id_;
    HttpTransactionOptions options_;
    HttpRequestOptions request_options_;

    DISALLOW_COPY_AND_ASSIGN(HttpTransaction);
};

}  // namespace transport
}  // namespace kwc

#endif  // KWCTOOLKIT_TRANSPORT_HTTP_TRANSACTION_H_
