# Copyright (c) 2022, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

if (WIN32)
  message(STATUS "Nothing to build for libjpeg just yet")
else()
  ExternalProject_Add(libjpeg
     URL ${libjpeg_path}
     URL_MD5 ${libjpeg_md5}
     INSTALL_DIR ${KWCToolkit_INSTALL_PREFIX}
     UPDATE_COMMAND ""
     BUILD_IN_SOURCE 1
     CONFIGURE_COMMAND ./configure --prefix=${KWCToolkit_INSTALL_PREFIX} --disable-shared)
endif()

ExternalProject_Get_Property(libjpeg install_dir)
set(libjpeg_name ${CMAKE_STATIC_LIBRARY_PREFIX}jpeg${CMAKE_STATIC_LIBRARY_SUFFIX})
set(libjpeg_ROOT ${install_dir} CACHE INTERNAL "")

file(MAKE_DIRECTORY ${libjpeg_ROOT}/include)
set(JPEG_INCLUDE_DIRS ${libjpeg_ROOT}/include)
set(JPEG_LIBRARIES ${libjpeg_ROOT}/lib/${libjpeg_name})

add_library(JPEG::JPEG STATIC IMPORTED GLOBAL)
add_dependencies(JPEG::JPEG libjpeg)
set_target_properties(JPEG::JPEG PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${JPEG_INCLUDE_DIRS}
  IMPORTED_LOCATION ${JPEG_LIBRARIES})

