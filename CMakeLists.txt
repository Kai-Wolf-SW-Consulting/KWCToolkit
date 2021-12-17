# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

cmake_minimum_required(VERSION 3.19..3.21 FATAL_ERROR)
project(KWCToolkit VERSION 0.0.1
  DESCRIPTION "Small utility library for cross-platform development"
  HOMEPAGE_URL "https://github.com/Kai-Wolf-SW-Consulting/KWCToolkit"
  LANGUAGES C CXX)

set(APPLICATION_NAME          "KWCToolkit")
set(APPLICATION_CODENAME      "${PROJECT_NAME}")
set(APPLICATION_COPYRIGHT     "Copyright (c) 2021 Kai Wolf - SW Consulting")
set(APPLICATION_VENDOR_ID     "me.kai-wolf")
set(APPLICATION_VENDOR_NAME   "Kai Wolf - SW Consulting")
set(APPLICATION_VENDOR_EMAIL  "mail@kai-wolf.me")
set(APPLICATION_VENDOR_URL    "${PROJECT_HOMEPAGE_URL}")
set(APPLICATION_ID            "${APPLICATION_VENDOR_ID}.${PROJECT_NAME}")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

include(BuildOption)
option(ENABLE_CPPCHECK              "Enable static analysis with cppcheck" OFF)
option(ENABLE_CLANG_TIDY            "Enable static analysis with clang-tidy" OFF)
option(ENABLE_INCLUDE_WHAT_YOU_USE  "Enable static analysis with include-what-you-use" OFF)
build_option(ENABLE_ASSERTS         "Enable asserts in codebase (Debug mode)" OFF)

## Setup project defaults
include(CompilerFlags)
include(InstallLocation)
include(StaticAnalyzer)

## Automatically creates a BUILD_TESTING option (default ON)
include(CTest)

add_subdirectory(third_party)
add_subdirectory(src)
