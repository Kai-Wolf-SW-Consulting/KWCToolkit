// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "transport/HttpRequest.h"

#include <map>
#include <ostream>
#include <utility>

#include "base/Check.h"
#include "serialization/DataReader.h"
#include "transport/HttpTransaction.h"

namespace kwc {
namespace transport {

const HttpRequest::HttpMethod HttpRequest::GET("GET");
const HttpRequest::HttpMethod HttpRequest::PUT("PUT");
const HttpRequest::HttpMethod HttpRequest::POST("POST");
const HttpRequest::HttpMethod HttpRequest::DELETE("DELETE");

const std::string HttpRequest::ContentType_HTML("text/html");
const std::string HttpRequest::ContentType_JSON("application/json");
const std::string HttpRequest::ContentType_TEXT("text/plain");

const std::string HttpRequest::HttpHeader_AUTHORIZATION("Authorization");
const std::string HttpRequest::HttpHeader_CONTENT_LENGTH("Content-Length");
const std::string HttpRequest::HttpHeader_CONTENT_TYPE("Content-Type");
const std::string HttpRequest::HttpHeader_HOST("Host");
const std::string HttpRequest::HttpHeader_TRANSFER_ENCODING("Transfer-Encoding");
const std::string HttpRequest::HttpHeader_USER_AGENT("Use-Agent");

// Helper class for encapsulating execution workflow state in order to support
// asynchronous requests.
// This helper class is used for both synchronous as well as asynchronous
// requests.
class HttpRequest::HttpRequestProcessor {
  public:
    explicit HttpRequestProcessor(HttpRequest* request)
        : request_(request),
          state_(request->getResponse()->getModifiableRequestState()),
          num_redirects_(0),
          num_retries(0),
          should_retry_(true) {}

    void executeSync() {
        request_->doExecute(request_->getResponse());

        // parse the response body
    }

    base::Status getStatus() const { return status_; }

  private:
    base::Status status_;
    HttpRequest* request_;
    HttpRequestState* state_;
    int num_redirects_;
    int num_retries;
    bool should_retry_;

    DISALLOW_COPY_AND_ASSIGN(HttpRequestProcessor);
};

HttpRequest::HttpRequest(HttpRequest::HttpMethod method, HttpTransaction* transaction)
    : http_method_(method),
      options_(transaction->getRequestOptions()),
      transaction_(transaction),
      response_(new HttpResponse),
      is_busy_(false) {
    content_reader_.reset(serialization::CreateUnmanagedInMemoryDataReader(""));
}

void HttpRequest::addHeader(const std::string& name, const std::string& value) {
    http_headers_[name] = value;
}
void HttpRequest::removeHeader(const std::string& name) {
    http_headers_.erase(name);
}

const std::string* HttpRequest::findHeaderValue(const std::string& name) const {
    auto found = http_headers_.find(name);
    return (found == http_headers_.end()) ? nullptr : &found->second;
}

void HttpRequest::setContentReader(serialization::DataReader* reader) {
    content_reader_.reset(reader);
}

void HttpRequest::setContentWriter(serialization::DataWriter* writer) {
    response_->setBodyWriter(writer);
}

void HttpRequest::setCallback(HttpRequestCallback* callback) {
    getModifiableRequestState()->setCallback(this, callback);
}

void HttpRequestState::setCallback(HttpRequest* request, HttpRequestCallback* callback) {
    std::lock_guard<std::mutex> guard(mutex_);
    request_ = request;
    callback_ = callback;
}

base::Status HttpRequest::execute() {
    auto* state = getResponse()->getModifiableRequestState();
    if (state->getStateCode() != HttpRequestState::QUEUED) {
        KWC_CHECK_EQ(HttpRequestState::UNSENT, state->getStateCode())
            << "Must call clear() before reusing";
    }

    HttpRequestProcessor exec(this);
    exec.executeSync();
    return exec.getStatus();
}

void HttpRequest::executeAsync(HttpRequestCallback* callback) {
    auto* state = getResponse()->getModifiableRequestState();
    KWC_CHECK_EQ(HttpRequestState::UNSENT, state->getStateCode())
        << "Must call clear() before reusing";
    if (callback != nullptr) {
        setCallback(callback);
    }
}

}  // namespace transport
}  // namespace kwc