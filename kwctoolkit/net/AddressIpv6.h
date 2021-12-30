// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_NET_ADDRESS_IPV6_H_
#define KWCTOOLKIT_NET_ADDRESS_IPV6_H_

#include <arpa/inet.h>
#include <netinet/in.h>

#include <cstring>
#include <stdexcept>
#include <string>

#include "kwctoolkit/base/Assert.h"
#include "kwctoolkit/base/Error.h"
#include "kwctoolkit/net/AddressTypes.h"

namespace kwc {
namespace net {

// This class provides the ability to create, use and manipulate ip version 6 addresses
class AddressIPv6 {
  public:
    AddressIPv6() : address_(IN6ADDR_ANY_INIT) {}

    AddressIPv6(const AddressIPv6& other) = default;

    AddressIPv6& operator=(const AddressIPv6& other) = default;

    // Returns the scope ID associated with the IPv6 address
    unsigned long getScopeID() const { return scope_id_; }

    // Modifies the scope ID associated with the IPv6 address
    void setScopeID(unsigned long id) { scope_id_ = id; }

    // create from an IPv6 address string
    static AddressOrError<AddressIPv6> fromString(const char* str) {
        AddressIPv6 tmp;

        if (inet_pton(AF_INET6, str, &tmp.address_) != 1) {
            return KWC_UNKNOWN_FORMAT_ERROR("Unknown IP address format");
        }

        return tmp;
    }

    // Get address as string
    std::string toString() const {
        char addr_str[INET6_ADDRSTRLEN];
        const char* addr = inet_ntop(AF_INET6, &address_, addr_str, INET6_ADDRSTRLEN);

        if (addr == nullptr) {
            return {};
        }

        return {addr};
    }

    // Compare two addresses for equality
    friend bool operator==(const AddressIPv6& a1, const AddressIPv6& a2) {
        return std::memcmp(&a1.address_, &a2.address_, sizeof(address_)) == 0 &&
               a1.scope_id_ == a2.scope_id_;
    }

    // Compare two addresses for inequality
    friend bool operator!=(const AddressIPv6& a1, const AddressIPv6& a2) {
        return std::memcmp(&a1.address_, &a2.address_, sizeof(address_)) != 0 ||
               a1.scope_id_ != a2.scope_id_;
    }

    friend std::ostream& operator<<(std::ostream& os, const AddressIPv6& a1) {
        return os << a1.toString();
    }

  private:
    // The underlying IPv6 address
    in6_addr address_;
    // Scope ID associated with the IPv6 address
    unsigned long scope_id_{0};
};

}  // namespace net
}  // namespace kwc

#endif  // KWCTOOLKIT_NET_ADDRESS_IPV6_H_
