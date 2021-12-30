// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_TRANSPORT_HTTP_REQUEST_H_
#define KWCTOOLKIT_TRANSPORT_HTTP_REQUEST_H_

#include <memory>
#include <string>

#include "kwctoolkit/base/Macros.h"
#include "kwctoolkit/base/Status.h"
#include "kwctoolkit/serialization/DataReader.h"
#include "kwctoolkit/transport/HttpResponse.h"
#include "kwctoolkit/transport/HttpTypes.h"

namespace kwc {
class DataWriter;

namespace transport {
class HttpTransaction;

// Declares an HTTP request to be sent to an HTTP server.
class HttpRequest {
  public:
    using HttpMethod = std::string;
    static const HttpMethod kGet;     // RFC 2616 GET
    static const HttpMethod kPut;     // RFC 2616 PUT
    static const HttpMethod kPost;    // RFC 2616 POST
    static const HttpMethod kDelete;  // RFC 2616 DELETE

    static const std::string kContentTypeHtml;  // text/html
    static const std::string kContentTypeJson;  // application/json
    static const std::string kContentTypeText;  // text/plain

    static const std::string kHttpHeaderAuthorization;     // Authorization
    static const std::string kHttpHeaderContentLength;     // Content-Length
    static const std::string kHttpHeaderContentType;       // Content-Type
    static const std::string kHttpHeaderHost;              // Host
    static const std::string kHttpHeaderTransferEncoding;  // Transfer-Encoding
    static const std::string kHttpHeaderUserAgent;         // User-Agent

    virtual ~HttpRequest() = default;

    // Returns modifiable options to configure instance-specific options.
    // These options should not be changed once execute() is called, otherwise
    // they won't take effect and can potentially harm the response processing
    HttpRequestOptions* getModifiableOptions() { return &options_; }

    HttpResponse* getResponse() const { return response_.get(); }

    const HttpRequestOptions getOptions() { return options_; }

    HttpMethod getHttpMethod() const { return http_method_; }

    const HttpRequestState& getRequestState() const { return response_->getCurrentRequestState(); }

    HttpRequestState* getModifiableRequestState() { return response_->getModifiableRequestState(); }

    void setContentReader(serialization::DataReader* reader);

    serialization::DataReader* getContentReader() const { return content_reader_.get(); }

    void setContentWriter(serialization::DataWriter* writer);

    HttpTransaction* getHttpTransaction() const { return transaction_; }

    void setUrl(const std::string& url) { url_ = url; }

    const std::string& getUrl() const { return url_; }

    void setCallback(HttpRequestCallback* callback);

    void addHeader(const std::string& name, const std::string& value);

    void removeHeader(const std::string& name);

    const std::string* findHeaderValue(const std::string& name) const;

    const HttpHeaderMap& getHeaders() const { return http_headers_; }

    base::Status execute();

    void executeAsync(HttpRequestCallback* callback);

  protected:
    friend class HttpRequestProcessor;
    HttpRequest(HttpMethod method, HttpTransaction* transaction);

    // Send the actual request for this message to the HTTP server
    virtual void doExecute(HttpResponse* response) = 0;

  private:
    class HttpRequestProcessor;
    friend class HttpRequestProcessor;

    HttpMethod http_method_;
    HttpRequestOptions options_;
    HttpTransaction* transaction_;
    std::unique_ptr<HttpResponse> response_;
    std::unique_ptr<serialization::DataReader> content_reader_;
    HttpHeaderMap http_headers_;
    std::string url_;
    bool is_busy_;

    DISALLOW_COPY_AND_ASSIGN(HttpRequest);
};
}  // namespace transport
}  // namespace kwc

#endif  // KWCTOOLKIT_TRANSPORT_HTTP_REQUEST_H_
