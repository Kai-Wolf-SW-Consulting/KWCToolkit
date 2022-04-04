# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

set(libpng_name ${CMAKE_STATIC_LIBRARY_PREFIX}png${CMAKE_STATIC_LIBRARY_SUFFIX})
if(CMAKE_HOST_WIN32)
  if(CMAKE_BUILD_TYPE MATCHES Debug)
    set(libpng_name libpng16_staticd${CMAKE_STATIC_LIBRARY_SUFFIX})
  else()
    set(libpng_name libpng16_static${CMAKE_STATIC_LIBRARY_SUFFIX})
  endif()
endif()

ExternalProject_Add(libpng
  URL ${libpng_path}
  URL_MD5 ${libpng_md5}
  INSTALL_DIR ${KWCToolkit_INSTALL_PREFIX}
  BUILD_BYPRODUCTS
    <INSTALL_DIR>/lib/${libpng_name}
  UPDATE_COMMAND ""
  DEPENDS zlib
  CMAKE_ARGS
    -Wno-dev ${KWCToolkit_DEFAULT_ARGS} -DPNG_SHARED=OFF
      -DPNG_EXECUTABLES=OFF -DPNG_TESTS=OFF -DPNG_STATIC=ON -DAWK=)

ExternalProject_Get_Property(libpng install_dir)
set(libpng_ROOT ${install_dir} CACHE INTERNAL "")

file(MAKE_DIRECTORY ${libpng_ROOT}/include)
set(PNG_INCLUDE_DIRS ${libpng_ROOT}/include)
set(PNG_LIBRARIES ${libpng_ROOT}/lib/${libpng_name})

add_library(PNG::PNG STATIC IMPORTED GLOBAL)
add_dependencies(PNG::PNG libpng)
set_target_properties(PNG::PNG PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${PNG_INCLUDE_DIRS}
  INTERFACE_LINK_LIBRARIES ${ZLIB_LIBRARIES}
  IMPORTED_LOCATION ${PNG_LIBRARIES})
