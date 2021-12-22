# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

# Hack to prevent CTest-added targets from cluttering up CLion's
# list of targets. Can be called during CMake configure step
# using: $ .. -DCMAKE_PROJECT_INCLUDE=cmake/CLionHack.cmake
# See https://stackoverflow.com/questions/44949354 for more info
set_property(GLOBAL PROPERTY CTEST_TARGETS_ADDED 1)