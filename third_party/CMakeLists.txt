# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

# Thirdparty libraries are built using ExternalProject_Add(), which per default
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

# Setu project dependencies
include(ExternalProject)
include(Projects)

option(USE_SYSTEM_GTEST "Use installed version of googletest library" OFF)
if(USE_SYSTEM_GTEST)
  find_package(GTest REQUIRED)
else()
  include(External_googletest)
endif()
