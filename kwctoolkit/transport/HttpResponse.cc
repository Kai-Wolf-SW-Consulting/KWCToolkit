// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/transport/HttpResponse.h"

#include <map>
#include <ostream>

#include <kwctoolkit/base/Logging.h>

namespace kwc {
namespace transport {
HttpResponse::HttpResponse() : request_state_(new HttpRequestState) {}

base::Status HttpResponse::getBody(std::string& body) {
    body.clear();
    if (body_reader_->getOffset() != 0 && !body_reader_->reset()) {
        LOGGING(base::WARNING) << "Could not reset HTTP response reader";
        return body_reader_->status();
    }

    body = body_reader_->readRemainingToString();
    auto status = body_reader_->status();
    body_reader_->reset();
    return status;
}

void HttpResponse::setBodyWriter(serialization::DataWriter* writer) {
    body_writer_.reset(writer);
}

void HttpResponse::setBodyReader(serialization::DataReader* reader) {
    body_reader_.reset(reader);
}

const std::string* HttpResponse::findHeaderValue(const std::string& name) const {
    std::string lname(name);
#if defined(KWC_OS_WINDOWS)
    std::transform(lname.begin(), lname.end(), lname.begin(), tolower);
#else
    std::transform(lname.begin(), lname.end(), lname.begin(), ::tolower);
#endif
    const auto found = headers_.find(lname);
    return (found == headers_.end()) ? nullptr : &found->second;
}

}  // namespace transport
}  // namespace kwc