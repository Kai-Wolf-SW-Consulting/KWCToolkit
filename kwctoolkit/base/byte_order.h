// Copyright (c) 2022, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_BYTE_ORDER_H_
#define KWCTOOLKIT_BASE_BYTE_ORDER_H_

#include <cstdint>

#include "kwctoolkit/base/integral_types.h"
#include "kwctoolkit/base/platform.h"

#if defined(KWC_OS_POSIX)
    #include <arpa/inet.h>
#endif

#include "kwctoolkit/base/compiler.h"

#if defined(KWC_OS_MACOS)
    #include <libkern/OSByteOrder.h>

    #define htobe16(v) OSSwapHostToBigInt16(v)
    #define htobe32(v) OSSwapHostToBigInt32(v)
    #define htobe64(v) OSSwapHostToBigInt64(v)
    #define be16toh(v) OSSwapBigToHostInt16(v)
    #define be32toh(v) OSSwapBigToHostInt32(v)
    #define be64toh(v) OSSwapBigToHostInt64(v)

    #define htole16(v) OSSwapHostToLittleInt16(v)
    #define htole32(v) OSSwapHostToLittleInt32(v)
    #define htole64(v) OSSwapHostToLittleInt64(v)
    #define le16toh(v) OSSwapLittleToHostInt16(v)
    #define le32toh(v) OSSwapLittleToHostInt32(v)
    #define le64toh(v) OSSwapLittleToHostInt64(v)

#elif defined(KWC_OS_WINDOWS)
    #include <WinSock2.h>

    #include <cstdlib>

    #if defined(KWC_ARCH_CPU_LITTLE_ENDIAN)
        #define htobe16(v) htons(v)
        #define htobe32(v) htonl(v)
        #define be16toh(v) ntohs(v)
        #define be32toh(v) ntohl(v)
        #define htole16(v) (v)
        #define htole32(v) (v)
        #define htole64(v) (v)
        #define le16toh(v) (v)
        #define le32toh(v) (v)
        #define le64toh(v) (v)
        #define htobe64(v) _byteswap_uint64(v)
        #define be64toh(v) _byteswap_uint64(v)

    #elif defined(KWC_ARCH_CPU_BIG_ENDIAN)
        #define htobe16(v) (v)
        #define htobe32(v) (v)
        #define be16toh(v) (v)
        #define be32toh(v) (v)
        #define htole16(v) __builtin_bswap16(v)
        #define htole32(v) __builtin_bswap32(v)
        #define htole64(v) __builtin_bswap64(v)
        #define le16toh(v) __builtin_bswap16(v)
        #define le32toh(v) __builtin_bswap32(v)
        #define le64toh(v) __builtin_bswap64(v)
        #define htobe64(v) (v)
        #define be64toh(v) (v)
    #else
        #error Either little or big endianess must be defined in byte_order.h
    #endif

#elif defined(KWC_OS_POSIX)
    #include <endian.h>
#else
    #error Missing byte order functions for this architecture in byte_order.h
#endif

namespace kwc {
namespace base {

// Reading and writing of little and big endian numbers from memory

inline void Set8(void* memory, size_t offset, uint8 v) {
    static_cast<uint8*>(memory)[offset] = v;
}

inline uint8 Get8(const void* memory, size_t offset) {
    return static_cast<const uint8*>(memory)[offset];
}

inline void SetBE16(void* memory, uint16 v) {
    *static_cast<uint16*>(memory) = htobe16(v);
}

inline void SetBE32(void* memory, uint32 v) {
    *static_cast<uint32*>(memory) = htobe32(v);
}

inline void SetBE64(void* memory, uint64 v) {
    *static_cast<uint64*>(memory) = htobe64(v);
}

inline uint16 GetBE16(const void* memory) {
    return be16toh(*static_cast<const uint16*>(memory));
}

inline uint32 GetBE32(const void* memory) {
    return be32toh(*static_cast<const uint32*>(memory));
}

inline uint64 GetBE64(const void* memory) {
    return be64toh(*static_cast<const uint64*>(memory));
}

inline void SetLE16(void* memory, uint16 v) {
    *static_cast<uint16*>(memory) = htole16(v);
}

inline void SetLE32(void* memory, uint32 v) {
    *static_cast<uint32*>(memory) = htole32(v);
}

inline void SetLE64(void* memory, uint64 v) {
    *static_cast<uint64*>(memory) = htole64(v);
}

inline uint16 GetLE16(const void* memory) {
    return le16toh(*static_cast<const uint16*>(memory));
}

inline uint32 GetLE32(const void* memory) {
    return le32toh(*static_cast<const uint32*>(memory));
}

inline uint64 GetLE64(const void* memory) {
    return le64toh(*static_cast<const uint64*>(memory));
}

inline uint16 HostToNetwork16(uint16 n) {
    return htobe16(n);
}

inline uint32 HostToNetwork32(uint32 n) {
    return htobe32(n);
}

inline uint64 HostToNetwork64(uint64 n) {
    return htobe64(n);
}

inline uint16 NetworkToHost16(uint16 n) {
    return be16toh(n);
}

inline uint32 NetworkToHost32(uint32 n) {
    return be32toh(n);
}

inline uint64 NetworkToHost64(uint64 n) {
    return be64toh(n);
}
}  // namespace base
}  // namespace kwc

#endif  // KWCTOOLKIT_BASE_BYTE_ORDER_H_
