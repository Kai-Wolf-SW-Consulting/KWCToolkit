# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

ExternalProject_Add(zlib
  URL ${zlib_path}
  URL_MD5 ${zlib_md5}
  INSTALL_DIR ${KWCToolkit_INSTALL_PREFIX}
  UPDATE_COMMAND ""
  CMAKE_ARGS -Wno-dev ${KWCToolkit_DEFAULT_ARGS} -DBUILD_SHARED_LIBS=OFF)

ExternalProject_Get_Property(zlib install_dir)
set(zlib_name ${CMAKE_STATIC_LIBRARY_PREFIX}z${CMAKE_STATIC_LIBRARY_SUFFIX})
set(zlib_ROOT ${install_dir} CACHE INTERNAL "")

file(MAKE_DIRECTORY ${zlib_ROOT}/include)
set(ZLIB_INCLUDE_DIRS ${zlib_ROOT}/include)
set(ZLIB_LIBRARIES ${zlib_ROOT}/lib/${zlib_name})

add_library(ZLIB::ZLIB STATIC IMPORTED GLOBAL)
add_dependencies(ZLIB::ZLIB zlib)
set_target_properties(ZLIB::ZLIB PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${ZLIB_INCLUDE_DIRS}
  IMPORTED_LOCATION ${ZLIB_LIBRARIES})
