# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

# Third party libraries are built using ExternalProject_Add(), which per default
# doesn't inherit any variables from the parent project, Thus, we set all
# necessary variables and settings from the parent.
set(KWCToolkit_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX})
set(KWCToolkit_DEFAULT_ARGS
  -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
  -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
  -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}
  -DCMAKE_C_COMPILER:STRING=${CMAKE_C_COMPILER}
  -DCMAKE_CXX_COMPILER:STRING=${CMAKE_CXX_COMPILER}
  -DCMAKE_GENERATOR:STRING=${CMAKE_GENERATOR}
  -DCMAKE_MAKE_PROGRAM:FILEPATH=${CMAKE_MAKE_PROGRAM}
  -DCMAKE_PREFIX_PATH:PATH=${KWCToolkit_INSTALL_PREFIX};${CMAKE_PREFIX_PATH}
  -DCMAKE_TOOLCHAIN_FILE:FILEPATH=${CMAKE_TOOLCHAIN_FILE})

# Setup project dependencies
include(ExternalProject)
include(Projects)

# Add ICU UTF8 handling library
add_library(third_party_icu
  icu/v60.1/icu_utf.cc
  icu/v60.1/icu_utf.h)
add_library(third_party::icu ALIAS third_party_icu)
target_include_directories(third_party_icu
  SYSTEM PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/icu/v60.1>
    $<INSTALL_INTERFACE:icu/v60.1>)
install(TARGETS third_party_icu EXPORT ${PROJECT_NAME}Targets)

option(USE_SYSTEM_GTEST "Use installed version of googletest library" OFF)
if(USE_SYSTEM_GTEST)
  find_package(GTest REQUIRED)
else()
  include(External_googletest)
endif()

option(USE_SYSTEM_ZLIB "Use installed version of zlib library" OFF)
if(USE_SYSTEM_ZLIB)
  find_package(ZLIB REQUIRED)
else()
  include(External_zlib)
endif()

option(USE_SYSTEM_LIBPNG "Use installed version of libpng library" OFF)
if(USE_SYSTEM_LIBPNG)
  find_package(PNG REQUIRED)
else()
  include(External_libpng)
endif()

option(USE_SYSTEM_LIBJPEG "Use installed version of libjpeg library" OFF)
if(USE_SYSTEM_LIBJPEG)
  find_package(JPEG REQUIRED)
else()
  include(External_libjpeg)
endif()