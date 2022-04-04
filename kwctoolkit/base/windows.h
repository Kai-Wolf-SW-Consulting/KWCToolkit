// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_BASE_WINDOWS_H
#define KWCTOOLKIT_BASE_WINDOWS_H

#include "kwctoolkit/base/platform.h"

// This header is intended to be a drop-in replacement for <Windows.h>,
// <WinSock2.h> and related headers as it undefines a lot of names that are
// used within KWCToolkit

#if defined(KWC_OS_WINDOWS)
    #include <cstdio>

    #ifndef __STDC__
        /* nolint */
        #define __STDC__ 1
        #pragma push_macro("_CRT_DECLARE_NONSTDC_NAMES")
        #ifdef _CRT_DECLARE_NONSTDC_NAMES
            #undef _CRT_DECLARE_NONSTDC_NAMES
        #endif
        #pragma push_macro("_CRT_INTERNAL_NONSTDC_NAMES")
        #undef _CRT_INTERNAL_NONSTDC_NAMES
        #include <direct.h>
        #include <io.h>
        #undef __STDC__
        #pragma pop_macro("_CRT_INTERNAL_NONSTDC_NAMES")
        #pragma pop_macro("_CRT_DECLARE_NONSTDC_NAMES")
    #else
        #include <direct.h>
        #include <io.h>
    #endif

    #if defined(min) || defined(max)
        #error The windows.h header needs to be included by this header, otherwise \
NOMINMAX needs to be defined before including it.
    #endif

    #ifndef NOMINMAX
        #define NOMINMAX 1
    #endif

    #include <WinSock2.h>
    #include <Windows.h>

    #ifdef ERROR
        #undef ERROR
    #endif

    #ifdef NO_ERROR
        #undef NO_ERROR
    #endif

    #ifdef STRICT
        #undef STRICT
    #endif

#endif

#endif  // KWCTOOLKIT_BASE_WINDOWS_H
