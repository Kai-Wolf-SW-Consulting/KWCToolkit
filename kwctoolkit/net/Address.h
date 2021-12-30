
// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_NET_ADDRESS_H_
#define KWCTOOLKIT_NET_ADDRESS_H_

#include "kwctoolkit/net/AddressIpv4.h"
#include "kwctoolkit/net/AddressIpv6.h"
#include "kwctoolkit/net/AddressTypes.h"

namespace kwc {
namespace net {

// This class provides the ability to use either IPv4 or IPv6 version addresses
class Address {
  public:
    // Default constructs an address from an IPv4 address
    Address() : address_ipv4_(), address_ipv6_() {}

    Address(const AddressIPv4& ipv4_addr) : address_ipv4_(ipv4_addr), address_ipv6_() {}

    Address(const AddressIPv6& ipv6_addr)
        : type_(Type::IPv6), address_ipv4_(), address_ipv6_(ipv6_addr) {}

    Address(const Address& other)

        = default;

    Address& operator=(const Address& other) = default;

    // Assign from an IPv4 address
    Address& operator=(const AddressIPv4& ipv4_addr) {
        type_ = Type::IPv4;
        address_ipv4_ = ipv4_addr;
        address_ipv6_ = AddressIPv6();
        return *this;
    }

    Address& operator=(const AddressIPv6& ipv6_addr) {
        type_ = Type::IPv6;
        address_ipv4_ = AddressIPv4();
        address_ipv6_ = ipv6_addr;
        return *this;
    }

    static bool isValid(const std::string& addr) {
        struct sockaddr_in sin;
        struct sockaddr_in6 sin6;
        if (inet_pton(AF_INET, addr.c_str(), &(sin.sin_addr)) == 1) {
            return true;
        } else if (inet_pton(AF_INET6, addr.c_str(), &(sin6.sin6_addr)) == 1) {
            return true;
        }
        return false;
    }

    bool isIPv4() const { return type_ == Type::IPv4; }

    bool isIPv6() const { return type_ == Type::IPv6; }

    std::string toString() const {
        if (type_ == Type::IPv4) {
            return address_ipv4_.toString();
        }

        return address_ipv6_.toString();
    }

    // Creates an address from either IPv4 or IPv6 address
    static AddressOrError<Address> fromString(const char* str) {
        Address tmp;

        auto from_v4 = AddressIPv4::fromString(str);
        if (from_v4.isSuccess()) {
            tmp.address_ipv4_ = from_v4.getSuccess();
            tmp.type_ = Type::IPv4;
            return tmp;
        }

        auto from_v6 = AddressIPv6::fromString(str);
        if (from_v6.isSuccess()) {
            tmp.address_ipv6_ = from_v6.getSuccess();
            tmp.type_ = Type::IPv6;
            return tmp;
        }

        return from_v4.isError() ? from_v4.getError() : from_v6.getError();
    }

    friend bool operator==(const Address& a1, const Address& a2) {
        if (a1.type_ != a2.type_) {
            return false;
        }

        if (a1.type_ == Type::IPv4) {
            return a1.address_ipv4_ == a2.address_ipv4_;
        }

        return a1.address_ipv4_ == a2.address_ipv4_;
    }

    friend bool operator!=(const Address& a1, const Address& a2) {
        if (a1.type_ != a2.type_) {
            return true;
        }

        if (a1.type_ == Type::IPv4) {
            return a1.address_ipv4_ != a2.address_ipv4_;
        }

        return a1.address_ipv6_ != a2.address_ipv6_;
    }

  private:
    enum class Type { IPv4, IPv6 } type_{Type::IPv4};
    AddressIPv4 address_ipv4_;
    AddressIPv6 address_ipv6_;
};

template <typename Elem, typename Traits>
std::basic_ostream<Elem, Traits>& operator<<(std::basic_ostream<Elem, Traits>& os,
                                             const Address& addr) {
    return os << addr.toString();
}

}  // namespace net
}  // namespace kwc

#endif  // KWCTOOLKIT_NET_ADDRESS_H_
