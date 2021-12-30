// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/transport/SimpleHttpTransaction.h"

#include <fcntl.h>

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ostream>
#include <string>
#include <utility>

#include "kwctoolkit/base/ArraySize.h"
#include "kwctoolkit/base/Check.h"
#include "kwctoolkit/base/Macros.h"
#include "kwctoolkit/serialization/DataWriter.h"
#include "kwctoolkit/strings/StringUtils.h"
#include "kwctoolkit/transport/HttpRequest.h"
#include "kwctoolkit/transport/HttpResponse.h"
#include "kwctoolkit/transport/HttpTransaction.h"
#include "kwctoolkit/transport/HttpTypes.h"

#if defined(KWC_OS_WINDOWS)
    #define _WINSOCK_DEPRECATED_NO_WARNINGS
    #define vsnprintf _vsnprintf
    #define _stricmp strcasecmp
    #include <WinSock2.h>
    #undef DELETE
#else
    #include <arpa/inet.h>
    #include <netdb.h>
    #include <netinet/in.h>
    #include <sys/select.h>
    #include <sys/socket.h>
    #include <sys/time.h>
    #include <unistd.h>
#endif

namespace kwc {
namespace serialization {
class DataReader;
}  // namespace serialization

namespace transport {
struct in_addr* GetAddressFromString(const char* address) {
    static struct in_addr saddr;
    saddr.s_addr = inet_addr(address);
    if (saddr.s_addr != -1) {
        return &saddr;
    }

    struct hostent* host = gethostbyname(address);
    return host != nullptr ? reinterpret_cast<struct in_addr*>(*host->h_addr_list) : nullptr;
}

// Returns true, if |socket| has data waiting to be read
bool DataAwaiting(int socket) {
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(socket, &fds);

    struct timeval tv = {4, 0};
    auto r = select(socket + 1, &fds, nullptr, nullptr, &tv);
    KWC_CHECK(r >= 0) << "select() call failed";

    return FD_ISSET(socket, &fds) != 0;
}

class SimpleHttpRequest : public HttpRequest {
  public:
    explicit SimpleHttpRequest(HttpRequest::HttpMethod method, SimpleHttpTransaction* transaction);
    ~SimpleHttpRequest() override;

  protected:
    void doExecute(HttpResponse* response) override;
};

// The |SimpleHttpProcessor| is an individual stateful request processor.
// It may be reused across requests but can process only one at a time.
class SimpleHttpProcessor {
  public:
    explicit SimpleHttpProcessor(HttpTransaction* transport)
        : transaction_(transport),
          response_state_(STATUSLINE),
          chunked_response_(false),
          chunk_bytes_left_(0),
          chunk_bytes_read_(0),
          chunk_length_(-1) {
        host_ = transaction_->getOptions().host();
        port_ = transaction_->getOptions().port();
    }

    void performRequest(SimpleHttpRequest* request, HttpResponse* response) {
        http_code_ = 0;
        send_content_reader_ = request->getContentReader();
        response_ = response;
        HttpRequestState* state = request->getModifiableRequestState();
        auto message = prepareRequestOptions(request);
        connect();
        send(reinterpret_cast<const unsigned char*>(message.c_str()), message.size());
        parseResponse(response->getBodyWriter());
        close();
    }

  private:
    enum HttpResponseState {
        STATUSLINE,  // status line is the first line of the response
        HEADERS,     // header lines
        BODY,        // body data (all or in chunks)
        CHUNK_LEN,   // expecting a chunk length (in hex)
        CHUNK_END,   // received chunk, now expecting a trailing blank line
        FOOTERS,     // optional trailing headers, in case of chunked messages
        COMPLETE,    // http response is complete;
    };

    std::string prepareRequestOptions(SimpleHttpRequest* request) {
        const std::string crlf("\r\n");
        std::string msg = request->getHttpMethod() + " " + request->getUrl() + " " + "HTTP/1.1";
        msg += crlf;
        msg += "Host: " + host_;
        msg += crlf;
        msg += "Accept-Encoding: identify";
        msg += crlf;
        msg += crlf;
        return msg;
    }

    int processDataChunked(serialization::DataWriter* body_writer,
                           std::string::iterator data,
                           int count) {
        KWC_CHECK(chunked_response_) << "processDataChunked() called with no chunk";
        auto n = count;
        if (n > chunk_bytes_left_) {
            n = chunk_bytes_left_;
        }

        body_writer->writeData(n, &*data);

        chunk_bytes_read_ += n;
        chunk_bytes_left_ -= n;
        KWC_CHECK_GE(chunk_bytes_left_, 0);
        if (chunk_bytes_left_ == 0) {
            // chunk completed. Soaking up trailing CRLF before next chunk
            response_state_ = CHUNK_END;
        }

        return n;
    }

    int processDataNonChunked(serialization::DataWriter* body_writer,
                              std::string::iterator data,
                              int count) {
        auto n = count;
        if (chunk_length_ != -1) {
            // we know how many bytes to expect
            auto remaining = chunk_length_ - chunk_bytes_read_;
            if (n > remaining) {
                n = remaining;
            }
        }

        body_writer->writeData(n, &*data);

        chunk_bytes_read_ += n;

        // Finish, if we know that we're done
        if (chunk_length_ != -1 && chunk_bytes_read_ == chunk_length_) {
            response_state_ = COMPLETE;
        }

        return n;
    }

    void parseResponse(serialization::DataWriter* body_writer) {
        std::string received_data = recv();
        KWC_CHECK(!received_data.empty()) << "recv() call failed";
        auto count = received_data.size();
        std::string line_buffer;

        auto dit = received_data.begin();
        while (dit != received_data.end() && response_state_ != COMPLETE) {
            if (accumulateHeaderLine(response_state_)) {
                while (dit != received_data.end()) {
                    auto c = (*dit++);
                    if (c == '\n') {  // whole line received
                        switch (response_state_) {
                            case STATUSLINE: processStatusLine(line_buffer); break;
                            case HEADERS: processHeaderLine(line_buffer); break;
                            case FOOTERS: processFooterLine(line_buffer); break;
                            case CHUNK_LEN: processChunkLengthLine(line_buffer); break;
                            case CHUNK_END:
                                KWC_CHECK(chunked_response_) << "No chunked response";
                                response_state_ = CHUNK_LEN;
                                break;
                            default: break;
                        }
                        line_buffer.clear();
                        break;
                    }
                    if (c != '\r') {  // we just ditch the CR part
                        line_buffer += c;
                    }
                }
            } else if (response_state_ == BODY) {
                auto bytes_used = 0;
                if (chunked_response_) {
                    bytes_used = processDataChunked(body_writer, dit, count);
                } else {
                    bytes_used = processDataNonChunked(body_writer, dit, count);
                }
                dit += bytes_used;
                count -= bytes_used;

                // body_writer->writeData(std::string(dit,
                // received_data.end()));
                // response_state_ = COMPLETE;
            }
        }
    }

    bool accumulateHeaderLine(HttpResponseState state) const {
        return state == STATUSLINE || state == HEADERS || state == CHUNK_LEN ||
               state == CHUNK_END || state == FOOTERS;
    }

    std::string recv() const {
        KWC_CHECK(socket_ > 0) << "Outstanding request but no connection";
        if (!DataAwaiting(socket_)) {
            return {""};
        }

        std::vector<unsigned char> buffer;
        std::vector<unsigned char> crlf{'0', '\r', '\n', '\r', '\n'};
        size_t ret;
        char data[1 << 12];
        while ((ret = ::recv(socket_, data, arraysize(data), 0)) != 0) {
#if defined(KWC_OS_WINDOWS)
            auto err = WSAGetLastError();
            if (err != 0 && (err == WSAEWOULDBLOCK))
                break;
#else
            if (ret == -1 && (errno == EAGAIN || errno == EWOULDBLOCK) && !DataAwaiting(socket_)) {
                break;
            }
#endif
            buffer.insert(buffer.end(), &data[0], &data[ret]);
            if (strings::EndsWith(buffer, crlf))
                break;
        }
        return {buffer.begin(), buffer.end()};
    }

    void processStatusLine(const std::string& line) {
        auto p = line.begin();

        // skip any leading space
        while ((*p != 0) && *p == ' ') {
            ++p;
        }

        // get version
        std::string version_string;
        while ((*p != 0) && *p != ' ') {
            version_string += *p++;
        }
        while ((*p != 0) && *p == ' ') {
            ++p;
        }

        // get status code_
        std::string status;
        while ((*p != 0) && *p != ' ') {
            status += *p++;
        }
        while ((*p != 0) && *p == ' ') {
            ++p;
        }

        // rest of the line is human-readable reason
        std::string reason;
        while (p != line.end() && (*p != 0)) {
            reason += *p++;
        }

        http_code_ = std::atoi(status.c_str());
        KWC_CHECK(HttpStatusCode::isValidError(http_code_)) << "Bad status line";

        response_state_ = HEADERS;
    }

    void processHeaderLine(const std::string& line) {
        if (line.empty()) {
            if (http_code_ == HttpStatusCode::CONTINUE) {
                // reset parsing because we expect a new status line
                response_state_ = STATUSLINE;
            } else {
                // received all the headers by now. Ready to start_ on the body
                // now
                beginBody();
            }
        } else {
            auto colon = line.find(':');
            KWC_CHECK_NE(std::string::npos, colon) << "Header=[" << line << "]";
            auto value = line.substr(colon + 1);
            value = strings::TrimString(value, " ", strings::TRIM_ALL);
            response_->addHeader(line.substr(0, colon), value);
        }
    }

    void processFooterLine(const std::string& line) {
        // we don't parse trailing header lines for now but just ignore them
        if (line.empty()) {
            response_state_ = COMPLETE;
        }
    }

    void processChunkLengthLine(const std::string& line) {
        // chunk length in hex at the beginning of the line
        chunk_bytes_left_ = strtol(line.c_str(), nullptr, 16);
        // if we received the whole body, we check for trailing headers
        response_state_ = chunk_bytes_left_ != 0 ? BODY : FOOTERS;
    }

    void beginBody() {
        const auto* chunk = response_->findHeaderValue("Transfer-encoding");
        if ((chunk != nullptr) && strings::StartsWith(*chunk, std::string("chunked"), false)) {
            chunked_response_ = true;
            chunk_bytes_left_ = -1;
        }

        // length supplied?
        auto* content_length = response_->findHeaderValue("Content-Length");
        if ((content_length != nullptr) && !chunked_response_) {
            chunk_length_ = std::atoi(content_length->c_str());
        }

        // check for several cases where we expect zero-length body
        if (http_code_ == HttpStatusCode::NO_CONTENT ||
            http_code_ == HttpStatusCode::NOT_MODIFIED ||
            HttpStatusCode::isInformational(http_code_)) {
            chunk_length_ = 0;
        }

        response_state_ = chunked_response_ ? CHUNK_LEN : BODY;
    }

    void connect() {
#if defined(KWC_OS_WINDOWS)
        WSADATA wsaData;
        KWC_CHECK(WSAStartup(MAKEWORD(2, 2), &wsaData) == 0) << "WSAStartup failed";
#endif

        auto addr = GetAddressFromString(host_.c_str());
        KWC_CHECK(addr) << "Invalid network address " << host_;

        sockaddr_in address;
        memset(reinterpret_cast<char*>(&address), 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_port = htons(port_);
        address.sin_addr.s_addr = addr->s_addr;

        socket_ = socket(AF_INET, SOCK_STREAM, 0);
        KWC_CHECK(socket_ >= 0) << "socket() call failed";
        KWC_CHECK(::connect(socket_, (sockaddr const*)&address, sizeof(address)) >= 0)
            << "connect() call failed";

// set non-blocking socket
#if defined(KWC_OS_WINDOWS)
        unsigned long non_blocking = 1;
        KWC_CHECK(ioctlsocket(socket_, FIONBIO, &non_blocking) == 0) << "ioctlsocket() call failed";
#else
        KWC_CHECK(fcntl(socket_, F_SETFL, fcntl(socket_, F_GETFL, 0) | O_NONBLOCK) == 0)
            << "fcntl() call failed";
#endif
    }

    void send(const unsigned char* buffer, size_t num_bytes) {
        if (socket_ < 0) {
            connect();
        }

        while (num_bytes > 0) {
            size_t n;
#if defined(KWC_OS_WINDOWS)
            n = ::send(socket_, (const char*)buffer, num_bytes, 0);
#else
            n = ::send(socket_, buffer, num_bytes, 0);
#endif
            KWC_CHECK(n >= 0) << "send() call failed";
            num_bytes -= n;
            buffer += n;
        }
    }

    void close() {
        if (socket_ >= 0) {
#if defined(KWC_OS_WINDOWS)
            shutdown(socket_, SD_SEND);
            ::closesocket(socket_);
#else
            ::close(socket_);
#endif
        }

        socket_ = -1;
    }

  private:
    HttpTransaction* transaction_;
    HttpResponse* response_;
    HttpResponseState response_state_;
    serialization::DataReader* send_content_reader_;
    int http_code_;
    int socket_;
    bool chunked_response_;
    long chunk_bytes_left_;
    long chunk_bytes_read_;
    long chunk_length_;
    std::string host_;
    unsigned short port_;
};

SimpleHttpRequest::SimpleHttpRequest(HttpRequest::HttpMethod method,
                                     SimpleHttpTransaction* transaction)
    : HttpRequest(std::move(method), transaction) {}

SimpleHttpRequest::~SimpleHttpRequest() {
#if defined(KWC_OS_WINDOWS)
    WSACleanup();
#endif
}

const char SimpleHttpTransaction::kTransactionIdentifier[] = "Simple";

void SimpleHttpRequest::doExecute(HttpResponse* response) {
    auto simple_transaction = static_cast<SimpleHttpTransaction*>(getHttpTransaction());
    SimpleHttpProcessor* processor = simple_transaction->acquireProcessor();
    processor->performRequest(this, response);
    simple_transaction->releaseProcessor(processor);
}

SimpleHttpTransaction::SimpleHttpTransaction(const HttpTransactionOptions& options)
    : HttpTransaction(options) {
    setId(kTransactionIdentifier);
}

std::unique_ptr<HttpRequest> SimpleHttpTransaction::createHttpRequest(
    const HttpRequest::HttpMethod& method) {
    return std::make_unique<SimpleHttpRequest>(method, this);
}

SimpleHttpTransactionFactory::SimpleHttpTransactionFactory() {
    setDefaultId(SimpleHttpTransaction::kTransactionIdentifier);
}

std::unique_ptr<HttpTransaction> SimpleHttpTransactionFactory::alloc(
    const HttpTransactionOptions& options) {
    return std::make_unique<SimpleHttpTransaction>(options);
}

SimpleHttpTransaction::~SimpleHttpTransaction() {
    for (SimpleHttpProcessor* processor : processors_) {
        delete processor;
    }
}

SimpleHttpProcessor* SimpleHttpTransaction::acquireProcessor() {
    {
        std::lock_guard<std::mutex> guard(mutex_);
        if (!processors_.empty()) {
            SimpleHttpProcessor* processor = processors_.back();
            processors_.pop_back();
            return processor;
        }
    }
    return new SimpleHttpProcessor(this);
}

void SimpleHttpTransaction::releaseProcessor(SimpleHttpProcessor* processor) {
    std::lock_guard<std::mutex> guard(mutex_);
    processors_.push_back(processor);
}

}  // namespace transport
}  // namespace kwc