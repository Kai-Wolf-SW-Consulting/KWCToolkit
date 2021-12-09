// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_MACROS_H_
#define KWCTOOLKIT_BASE_MACROS_H_

#include <memory>

// Inside the declaration of a class will make it unassignable
#define DISALLOW_ASSIGN(TypeName) void operator=(const TypeName&) = delete

// Inside the declaration of a class will make it uncopyable and unassignable
#if __cplusplus < 201103L
    #define DISALLOW_COPY_AND_ASSIGN(TypeName) \
        TypeName(const TypeName&);             \
        void operator=(const TypeName&)
#else
    #define DISALLOW_COPY_AND_ASSIGN(TypeName) \
        TypeName(const TypeName&) = delete;    \
        TypeName& operator=(const TypeName&) = delete
#endif

// Inside the declaration of a class will make it un-default constructible
#define DISALLOW_DEFAULT_CTOR(TypeName) TypeName() = delete

#define DISALLOW_IMPLICIT_CONSTRUCTOR(TypeName) \
    TypeName() = delete;                        \
    DISALLOW_COPY_AND_ASSIGN(TypeName)

// Concatenate symbols s1 and s2
#define CONCATENATE_IMPL(s1, s2) s1##s2
#define CONCATENATE(s1, s2) CONCATENATE_IMPL(s1, s2)

#ifdef __COUNTER__
    #define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __COUNTER__)
#else
    #define ANONYMOUS_VARIABLE(str) CONCATENATE(str, __LINE__)
#endif

#endif  // KWCTOOLKIT_BASE_MACROS_H_
