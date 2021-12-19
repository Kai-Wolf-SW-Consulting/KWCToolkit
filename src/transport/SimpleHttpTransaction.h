// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_TRANSPORT_SIMPLE_HTTP_TRANSACTION_H_
#define KWCTOOLKIT_TRANSPORT_SIMPLE_HTTP_TRANSACTION_H_

#include <memory>
#include <vector>

#include "base/Macros.h"
#include "transport/HttpTransaction.h"

namespace kwc {
namespace transport {
class SimpleHttpProcessor;

class SimpleHttpTransactionFactory : public HttpTransactionFactory {
  public:
    SimpleHttpTransactionFactory();
    ~SimpleHttpTransactionFactory() override = default;

  protected:
    std::unique_ptr<HttpTransaction> alloc(const HttpTransactionOptions& options) override;

  private:
    SimpleHttpTransactionFactory(const HttpTransactionFactory&) = delete;
    SimpleHttpTransactionFactory operator=(const HttpTransactionFactory&) = delete;
};

class SimpleHttpTransaction : public HttpTransaction {
  public:
    explicit SimpleHttpTransaction(const HttpTransactionOptions& options);
    ~SimpleHttpTransaction() override;

    std::unique_ptr<HttpRequest> createHttpRequest(const HttpRequest::HttpMethod& method) override;

    // The default id attribute identifying simple transaction instances
    static const char kTransactionIdentifier[];

  private:
    // Acquires a |SimpleHttpProcessor| based upon the active transaction
    // configuration.
    // The caller has exclusive use of the given processor until
    // |releaseProcessor()| is called.
    SimpleHttpProcessor* acquireProcessor();
    void releaseProcessor(SimpleHttpProcessor* processor);
    std::mutex mutex_;
    std::vector<SimpleHttpProcessor*> processors_;

    friend class SimpleHttpRequest;
    DISALLOW_COPY_AND_ASSIGN(SimpleHttpTransaction);
};

}  // namespace transport
}  // namespace kwc

#endif  // KWCTOOLKIT_TRANSPORT_SIMPLE_HTTP_TRANSACTION_H_
