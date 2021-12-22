// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/utils/ColorPrint.h"

#include <cstdarg>
#include <cstdio>

#include <kwctoolkit/base/CmdlineFlags.h>
#include <kwctoolkit/base/Platform.h>  // IWYU pragma: keep

#if defined(KWC_OS_WINDOWS)
    #include <Windows.h>
#endif

DECLARE_bool(color_print);

namespace kwc {
namespace utils {
namespace {
#if defined(KWC_OS_WINDOWS)
using PlatformColorCode = WORD;
#else
using PlatformColorCode = const char*;
#endif

PlatformColorCode GetPlatformColorCode(LogColor color) {
#if defined(KWC_OS_WINDOWS)
    switch (color) {
        case COLOR_RED: return FOREGROUND_RED;
        case COLOR_GREEN: return FOREGROUND_GREEN;
        case COLOR_YELLOW: return FOREGROUND_RED | FOREGROUND_GREEN;
        case COLOR_BLUE: return FOREGROUND_BLUE;
        case COLOR_MAGENTA: return FOREGROUND_BLUE | FOREGROUND_RED;
        case COLOR_CYAN: return FOREGROUND_BLUE | FOREGROUND_GREEN;
        case COLOR_WHITE:  // fall through to default
        default: return 0;
    }
#else
    switch (color) {
        case COLOR_RED: return "1";
        case COLOR_GREEN: return "2";
        case COLOR_YELLOW: return "3";
        case COLOR_BLUE: return "4";
        case COLOR_MAGENTA: return "5";
        case COLOR_CYAN: return "6";
        case COLOR_WHITE: return "7";
        default: return nullptr;
    };
#endif
}

}  // namespace

void ColorPrint(LogColor color, const char* format, ...) {
    va_list args;
    va_start(args, format);

    if (!FLAGS_color_print) {
        vprintf(format, args);
        va_end(args);
        return;
    }

#if defined(KWC_OS_WINDOWS)
    auto stdout_handle = GetStdHandle(STD_OUTPUT_HANDLE);

    // Get the current text color
    CONSOLE_SCREEN_BUFFER_INFO buffer_info;
    GetConsoleScreenBufferInfo(stdout_handle, &buffer_info);
    auto old_color_attrs = buffer_info.wAttributes;

    // We need to flush the stream buffer into the console before each
    // SetConsoleTextAttribute call lest it affect the text that is already
    // printed but has not yet reached the console
    fflush(stdout);
    SetConsoleTextAttribute(stdout_handle, GetPlatformColorCode(color) | FOREGROUND_INTENSITY);
    vprintf(format, args);
    fflush(stdout);

    // Restore the text color
    SetConsoleTextAttribute(stdout_handle, old_color_attrs);
#else
    auto color_code = GetPlatformColorCode(color);
    if (color_code)
        fprintf(stdout, "\033[0;3%sm", color_code);
    vprintf(format, args);
    // Reset the terminal to default
    printf("\033[m");
#endif
    va_end(args);
}

}  // namespace utils
}  // namespace kwc
