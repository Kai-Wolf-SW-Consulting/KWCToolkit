// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_NET_ADDRESS_IPV4_H_
#define KWCTOOLKIT_NET_ADDRESS_IPV4_H_

#include <arpa/inet.h>
#include <netinet/in.h>

#include <stdexcept>
#include <string>

#include "kwctoolkit/base/Error.h"
#include "kwctoolkit/net/AddressTypes.h"

namespace kwc {
namespace net {

// This class provides the ability to create, use and manipulate ip version 4 addresses
class AddressIPv4 {
  public:
    // Default constructor for IPv4 address
    // The address will be 0.0.0.0 per default
    AddressIPv4() { address_.s_addr = 0; }

    AddressIPv4(const AddressIPv4& other) : address_(other.address_) {}

    AddressIPv4& operator=(const AddressIPv4& other) {
        address_ = other.address_;
        return *this;
    }

    // Creates an address from an IPv4 address string in dotted decimal form
    static AddressOrError<AddressIPv4> fromString(const char* str) {
        AddressIPv4 tmp;

        if (inet_pton(AF_INET, str, &tmp.address_) != 1) {
            return KWC_UNKNOWN_FORMAT_ERROR("Unknown IP address format");
        }

        return tmp;
    }

    // Get address as string in dotted decimal format
    std::string toString() const {
        char addrStr[INET_ADDRSTRLEN];
        const char* addr = inet_ntop(AF_INET, &address_, addrStr, INET_ADDRSTRLEN);

        if (addr == nullptr) {
            return std::string();
        }

        return std::string(addr);
    }

    // Compare two addresses for equality
    friend bool operator==(const AddressIPv4& a1, const AddressIPv4& a2) {
        return a1.address_.s_addr == a2.address_.s_addr;
    }

    // Compare two addresses for inequality
    friend bool operator!=(const AddressIPv4& a1, const AddressIPv4& a2) {
        return a1.address_.s_addr != a2.address_.s_addr;
    }

    friend std::ostream& operator<<(std::ostream& os, const AddressIPv4& a1) {
        return os << a1.toString();
    }

  private:
    // The underlying IPv4 address
    in_addr address_;
};
}  // namespace net
}  // namespace kwc

#endif  // KWCTOOLKIT_NET_ADDRESS_IPV4_H_
