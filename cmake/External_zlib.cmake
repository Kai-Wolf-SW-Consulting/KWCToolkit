# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

set(zlib_name ${CMAKE_STATIC_LIBRARY_PREFIX}z${CMAKE_STATIC_LIBRARY_SUFFIX})
if(CMAKE_HOST_WIN32)
    if(CMAKE_BUILD_TYPE MATCHES Debug)
        set(zlib_name zlibd${CMAKE_STATIC_LIBRARY_SUFFIX})
    else()
        set(zlib_name zlib${CMAKE_STATIC_LIBRARY_SUFFIX})
    endif()
endif()

ExternalProject_Add(zlib
  URL ${zlib_path}
  URL_MD5 ${zlib_md5}
  INSTALL_DIR ${KWCToolkit_INSTALL_PREFIX}
  BUILD_BYPRODUCTS
    <INSTALL_DIR>/lib/${zlib_name}
  UPDATE_COMMAND ""
  CMAKE_ARGS -Wno-dev ${KWCToolkit_DEFAULT_ARGS} -DBUILD_SHARED_LIBS=OFF)

ExternalProject_Get_Property(zlib install_dir)
set(zlib_ROOT ${install_dir} CACHE INTERNAL "")

file(MAKE_DIRECTORY ${zlib_ROOT}/include)
set(ZLIB_INCLUDE_DIRS ${zlib_ROOT}/include)
set(ZLIB_LIBRARIES ${zlib_ROOT}/lib/${zlib_name})

add_library(ZLIB::ZLIB STATIC IMPORTED GLOBAL)
add_dependencies(ZLIB::ZLIB zlib)
set_target_properties(ZLIB::ZLIB PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${ZLIB_INCLUDE_DIRS}
  IMPORTED_LOCATION ${ZLIB_LIBRARIES})
