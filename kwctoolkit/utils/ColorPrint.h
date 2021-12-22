// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_UTILS_COLOR_PRINT_H_
#define KWCTOOLKIT_UTILS_COLOR_PRINT_H_

namespace kwc {
namespace utils {

enum LogColor {
    COLOR_DEFAULT,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_WHITE
};

void ColorPrint(LogColor color, const char* format, ...);

}  // namespace utils
}  // namespace kwc

#endif  // KWCTOOLKIT_UTILS_COLOR_PRINT_H_
