# Copyright (c) 2022, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

set(KWCToolkit_SUPPORTED_BUILD_TYPES "Debug" "Release" "RelWithDebInfo")

set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING
  "Select build type, options are: ${KWCToolkit_SUPPORTED_BUILD_TYPES}" FORCE)

# Set default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${KWCToolkit_SUPPORTED_BUILD_TYPES})

if (NOT ${CMAKE_BUILD_TYPE} IN_LIST KWCToolkit_SUPPORTED_BUILD_TYPES)
  message(FATAL_ERROR "Unsupported build type: ${CMAKE_BUILD_TYPE}")
endif()
