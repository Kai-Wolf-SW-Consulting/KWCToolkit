// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_TRANSPORT_HTTP_RESPONSE_H_
#define KWCTOOLKIT_TRANSPORT_HTTP_RESPONSE_H_

#include <algorithm>
#include <cctype>
#include <memory>
#include <string>
#include <utility>

#include "kwctoolkit/base/Macros.h"
#include "kwctoolkit/base/Status.h"
#include "kwctoolkit/serialization/DataReader.h"
#include "kwctoolkit/serialization/DataWriter.h"
#include "kwctoolkit/transport/HttpTypes.h"

namespace kwc {
namespace transport {

class HttpResponse {
  public:
    HttpResponse();
    virtual ~HttpResponse() = default;

    const HttpRequestState& getCurrentRequestState() const { return *request_state_.get(); }

    HttpRequestState* getModifiableRequestState() { return request_state_.get(); }

    HttpRequestState::StateCode getRequestStateCode() const {
        return request_state_->getStateCode();
    }

    void setBodyWriter(serialization::DataWriter* writer);

    serialization::DataWriter* getBodyWriter() { return body_writer_.get(); }

    const HttpHeaderMap& getHeaders() const { return headers_; }

    void addHeader(const std::string& name, const std::string& value) {
        std::string lname(name);
#if defined(KWC_OS_WINDOWS)
        std::transform(lname.begin(), lname.end(), lname.begin(), tolower);
#else
        std::transform(lname.begin(), lname.end(), lname.begin(), ::tolower);
#endif
        headers_.insert(std::make_pair(lname, value));
    }

    const std::string* findHeaderValue(const std::string& name) const;

    void clearHeaders() { headers_.clear(); }

    void setBodyReader(serialization::DataReader* reader);

    serialization::DataReader* getBodyReader() { return body_reader_.get(); }

    base::Status getBody(std::string& body);

    void setHttpCode(int code) { request_state_->setHttpCode(code); }

  private:
    std::unique_ptr<HttpRequestState> request_state_;
    std::unique_ptr<serialization::DataReader> body_reader_;
    std::unique_ptr<serialization::DataWriter> body_writer_;
    HttpHeaderMap headers_;

    DISALLOW_COPY_AND_ASSIGN(HttpResponse);
};

}  // namespace transport
}  // namespace kwc

#endif  // KWCTOOLKIT_TRANSPORT_HTTP_RESPONSE_H_
