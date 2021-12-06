# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

add_library(kwc_base
  array_copy.h
  status.cc
  status.h)

add_library(kwc::base ALIAS kwc_base)

target_include_directories(kwc_base
  PUBLIC
    $<BUILD_INTERFACE:${KWCToolkit_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>)

install(TARGETS kwc_base
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR}
  LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR})
install(FILES ${PUBLIC_BASE_HEADER} 
  DESTINATION ${INSTALL_INCLUDE_DIR}/kwctoolkit/base)
install(EXPORT ${PROJECT_NAME}Targets NAMESPACE kwc:: DESTINATION lib/cmake/)

if(BUILD_TESTING)
  include(GoogleTest)
  add_executable(kwc_base_test
    array_copy_test.cc)

  target_link_libraries(kwc_base_test
    PRIVATE kwc::base GTest::gtest_main GTest::gtest)
  gtest_discover_tests(kwc_base_test)
endif()
