# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

set(googletest_name ${CMAKE_STATIC_LIBRARY_PREFIX}gtest${CMAKE_STATIC_LIBRARY_SUFFIX})
set(googletest_name_debug ${CMAKE_STATIC_LIBRARY_PREFIX}gtestd${CMAKE_STATIC_LIBRARY_SUFFIX})

set(googletest_main_name ${CMAKE_STATIC_LIBRARY_PREFIX}gtest_main${CMAKE_STATIC_LIBRARY_SUFFIX})
set(googletest_main_name_debug ${CMAKE_STATIC_LIBRARY_PREFIX}gtest_maind${CMAKE_STATIC_LIBRARY_SUFFIX})

set(googlemock_name ${CMAKE_STATIC_LIBRARY_PREFIX}gmock${CMAKE_STATIC_LIBRARY_SUFFIX})
set(googlemock_name_debug ${CMAKE_STATIC_LIBRARY_PREFIX}gmockd${CMAKE_STATIC_LIBRARY_SUFFIX})

set(googlemock_main_name ${CMAKE_STATIC_LIBRARY_PREFIX}gmock_main${CMAKE_STATIC_LIBRARY_SUFFIX})
set(googlemock_main_name_debug ${CMAKE_STATIC_LIBRARY_PREFIX}gmock_maind${CMAKE_STATIC_LIBRARY_SUFFIX})

find_package(Threads REQUIRED)

ExternalProject_Add(googletest
  URL ${googletest_path}
  URL_MD5 ${googletest_md5}
  # Only build googletest if something actually needs it (BUILD_TESTING=ON)
  EXCLUDE_FROM_ALL 1
  INSTALL_DIR ${KWCToolkit_INSTALL_PREFIX}
  UPDATE_COMMAND ""
  CMAKE_ARGS -Wno-dev ${KWCToolkit_DEFAULT_ARGS} -DBUILD_SHARED_LIBS=OFF
    -Dgtest_force_shared_crt=ON)

ExternalProject_Get_Property(googletest install_dir)
set(googletest_ROOT ${install_dir} CACHE INTERNAL "")
set(googletest_INCLUDE_DIR ${googletest_ROOT}/include/)

file(MAKE_DIRECTORY ${googletest_ROOT}/include/gtest)
set(googletest_LIBRARY ${googletest_ROOT}/lib/${googletest_name})
set(googletest_LIBRARY_DEBUG ${googletest_ROOT}/lib/${googletest_name_debug})
set(googletest_main_LIBRARY ${googletest_ROOT}/lib/${googletest_main_name})
set(googletest_main_LIBRARY_DEBUG ${googletest_ROOT}/lib/${googletest_main_name_debug})

file(MAKE_DIRECTORY ${googletest_ROOT}/include/gmock)
set(googlemock_LIBRARY ${googletest_ROOT}/lib/${googlemock_name})
set(googlemock_LIBRARY_DEBUG ${googletest_ROOT}/lib/${googlemock_name_debug})
set(googlemock_main_LIBRARY ${googletest_ROOT}/lib/${googlemock_main_name})
set(googlemock_main_LIBRARY_DEBUG ${googletest_ROOT}/lib/${googlemock_main_name_debug})

add_library(GTest::gtest STATIC IMPORTED GLOBAL)
add_dependencies(GTest::gtest googletest)
set_target_properties(GTest::gtest PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${googletest_INCLUDE_DIR}
  IMPORTED_LOCATION ${googletest_LIBRARY}
  IMPORTED_LOCATION_DEBUG ${googletest_LIBRARY_DEBUG}
  IMPORTED_LINK_INTERFACE_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}")

add_library(GTest::gtest_main STATIC IMPORTED GLOBAL)
add_dependencies(GTest::gtest_main googletest)
set_target_properties(GTest::gtest_main PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${googletest_INCLUDE_DIR}
  IMPORTED_LOCATION ${googletest_main_LIBRARY}
  IMPORTED_LOCATION_DEBUG ${googletest_main_LIBRARY_DEBUG}
  IMPORTED_LINK_INTERFACE_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}")

add_library(GTest::gmock STATIC IMPORTED GLOBAL)
add_dependencies(GTest::gmock googletest)
set_target_properties(GTest::gmock PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${googletest_INCLUDE_DIR}
  IMPORTED_LOCATION ${googlemock_LIBRARY}
  IMPORTED_LOCATION_DEBUG ${googlemock_LIBRARY_DEBUG}
  IMPORTED_LINK_INTERFACE_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}")

add_library(GTest::gmock_main STATIC IMPORTED GLOBAL)
add_dependencies(GTest::gmock_main googletest)
set_target_properties(GTest::gmock_main PROPERTIES
  INTERFACE_INCLUDE_DIRECTORIES ${googletest_INCLUDE_DIR}
  IMPORTED_LOCATION ${googlemock_main_LIBRARY}
  IMPORTED_LOCATION_DEBUG ${googlemock_main_LIBRARY_DEBUG}
  IMPORTED_LINK_INTERFACE_LIBRARIES "${CMAKE_THREAD_LIBS_INIT}")
