// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_TRANSPORT_HTTP_REQUEST_H_
#define KWCTOOLKIT_TRANSPORT_HTTP_REQUEST_H_

#include <memory>
#include <string>

#include "base/macros.h"
#include "base/status.h"
#include "serialization/data_reader.h"
#include "transport/http_response.h"
#include "transport/http_types.h"

namespace kwc {
class DataWriter;

namespace transport {
class HttpTransaction;

// Declares an HTTP request to be sent to an HTTP server.
class HttpRequest {
  public:
    using HttpMethod = std::string;
    static const HttpMethod GET;     // RFC 2616 GET
    static const HttpMethod PUT;     // RFC 2616 PUT
    static const HttpMethod POST;    // RFC 2616 POST
    static const HttpMethod DELETE;  // RFC 2616 DELETE

    static const std::string ContentType_HTML;  // text/html
    static const std::string ContentType_JSON;  // application/json
    static const std::string ContentType_TEXT;  // text/plain

    static const std::string HttpHeader_AUTHORIZATION;      // Authorization
    static const std::string HttpHeader_CONTENT_LENGTH;     // Content-Length
    static const std::string HttpHeader_CONTENT_TYPE;       // Content-Type
    static const std::string HttpHeader_HOST;               // Host
    static const std::string HttpHeader_TRANSFER_ENCODING;  // Transfer-Encoding
    static const std::string HttpHeader_USER_AGENT;         // User-Agent

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
