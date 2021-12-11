// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_TRANSPORT_HTTP_TYPES_H_
#define KWCTOOLKIT_TRANSPORT_HTTP_TYPES_H_

#include <cstdint>
#include <map>
#include <string>

#include "base/callback.h"
#include "base/integral_types.h"
#include "base/macros.h"
#include "base/status.h"

namespace kwc {
namespace transport {
class HttpRequest;
class HttpResponse;

// Collection of HTTP headers (no repeating)
using HttpHeaderMap = std::map<std::string, std::string>;

// Denotes a callback function that takes an HttpRequest parameter.
// Used for notification on asynchronous requests.
using HttpRequestCallback = Callback1<HttpRequest*>;

class HttpStatusCode {
  public:
    // Symbolic names for common HTTP status codes.
    // Note that the following list is not a complete enumeration of all
    // possible HTTP status code values, but only the ones which are of
    // particular interest within this library.
    // See http://www.w3.org/Protocols/rfc2616/rfc2616-sec10.html for a
    // complete list of all standard status codes
    enum HttpStatus {
        // 1xx informational
        CONTINUE = 100,
        SWITCHING_PROTOCOLS = 101,
        PROCESSING = 102,

        // 2xx successful
        OK = 200,
        CREATED = 201,
        ACCEPTED = 202,
        NON_AUTHORITATIVE_INFORMATION = 203,
        NO_CONTENT = 204,
        RESET_CONTENT = 205,
        PARTIAL_CONTENT = 206,
        MULTI_STATUS = 207,
        IM_USED = 226,

        // 3xx redirection
        MULTIPLE_CHOICES = 300,
        MOVED_PERMANENTLY = 301,
        FOUND = 302,
        SEE_OTHER = 303,
        NOT_MODIFIED = 304,
        USE_PROXY = 305,
        TEMPORARY_REDIRECT = 307,

        // 4xx client error
        BAD_REQUEST = 400,
        UNAUTHORIZED = 401,
        PAYMENT_REQUIRED = 402,
        FORBIDDEN = 403,
        NOT_FOUND = 404,
        METHOD_NOT_ALLOWED = 405,
        NOT_ACCEPTABLE = 406,
        PROXY_AUTHENTICATION_REQUIRED = 407,
        REQUEST_TIMEOUT = 408,
        CONFLICT = 409,
        GONE = 410,
        LENGTH_REQUIRED = 411,
        PRECONDITION_FAILED = 412,
        REQUEST_ENTITY_TOO_LARGE = 413,
        REQUEST_URI_TOO_LONG = 414,
        UNSUPPORTED_MEDIA_TYPE = 415,
        REQUESTED_RANGE_NOT_SATISFIABLE = 416,
        EXPECTATION_FAILED = 417,
        UNPROCESSABLE_ENTITY = 422,
        LOCKED = 423,
        FAILED_DEPENDENCY = 424,
        UPGRADE_REQUIRED = 426,

        // 5xx server error
        INTERNAL_SERVER_ERROR = 500,
        NOT_IMPLEMENTED = 501,
        BAD_GATEWAY = 502,
        SERVICE_UNAVAILABLE = 503,
        GATEWAY_TIMEOUT = 504,
        HTTP_VERSION_NOT_SUPPORTED = 505,
        INSUFFICIENT_STORAGE = 507,
        NOT_EXTENDED = 510,
    };

    static bool IsValidError(int http_code) { return 100 <= http_code && http_code < 600; }

    static bool IsServerError(int http_code) { return 500 <= http_code && http_code < 600; }

    static bool IsClientError(int http_code) { return 400 <= http_code && http_code < 500; }

    static bool IsRedirect(int http_code) {
        return 300 <= http_code && http_code <= 307 && http_code != 304;
    }

    static bool IsOk(int http_code) { return 200 <= http_code && http_code < 300; }

    static bool IsInformational(int http_code) { return 100 <= http_code && http_code < 200; }

  private:
    DISALLOW_IMPLICIT_CONSTRUCTOR(HttpStatusCode);
};

// Denotes the current state of a http request lifecycle.
// This class includes the |StateCode| as its state machine progress as well
// as status and response data.
// The state is shared between an HttpRequest and its HttpResponse such that
// its accessible by either.
class HttpRequestState {
  public:
    enum StateCode {
        UNSENT,          // request has not been sent yet
        QUEUED,          // request has been queued (async) but not sent yet
        PENDING,         // request has been sent but no response received
        COMPLETED,       // a valid http response has been received
        COULD_NOT_SEND,  // an error prevented request from being sent
        TIMED_OUT,       // request timed out before a response arrived
        CANCELLED,       // request has been cancelled before it was sent
        ABORTED,         // signal callback will never be called
        _NUM_STATES_     // internal marker used to count the number of state
    };

    HttpRequestState() : state_code_(UNSENT), http_code_(0) {}

    StateCode getStateCode() const {
        std::lock_guard<std::mutex> guard(mutex_);
        return state_code_;
    }

    void setHttpCode(int http_code) { http_code_ = http_code; }

    int getHttpCode() const { return http_code_; }

  private:
    bool hasCallback() {
        std::lock_guard<std::mutex> guard(mutex_);
        return callback_ != nullptr;
    }

    void setCallback(HttpRequest* request, HttpRequestCallback* callback);

  private:
    friend class HttpRequest;
    HttpRequestCallback* callback_;
    mutable std::mutex mutex_;
    StateCode state_code_;
    base::Status transaction_status_;
    int http_code_;
    HttpRequest* request_;
};

// Specifies per-request options that control its behaviour.
class HttpRequestOptions {
  public:
    HttpRequestOptions() = default;

    void clearTimeout() { timeout_ms_ = 0; }

    void setTimeout(int64 ms) { timeout_ms_ = ms; }

    int64 timeout() const { return timeout_ms_; }

    bool destroyWhenDone() const { return destroy_when_done_; }

    void setDestroyWhenDone(bool toggle) { destroy_when_done_ = toggle; }

    void setPriority(unsigned int priority) { priority_ = priority; }

    unsigned int priority() const { return priority_; }

  private:
    int64 timeout_ms_;
    bool destroy_when_done_;
    unsigned int priority_;
};
}  // namespace transport
}  // namespace kwc

#endif  // KWCTOOLKIT_TRANSPORT_HTTP_TYPES_H_
