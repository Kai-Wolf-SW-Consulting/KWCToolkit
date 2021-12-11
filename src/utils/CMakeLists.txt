# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

add_library(kwc_utils
  base_64.cc
  base_64.h
  benchmark.h
  color_print.cc
  color_print.h)

add_library(kwc::utils ALIAS kwc_utils)

target_include_directories(kwc_utils
  PUBLIC
    $<BUILD_INTERFACE:${KWCToolkit_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(kwc_utils
  PUBLIC kwc::base)

install(TARGETS kwc_utils
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR}
  LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR})
install(FILES ${PUBLIC_UTILS_HEADER}
  DESTINATION ${INSTALL_INCLUDE_DIR}/kwctoolkit/utils)
install(EXPORT ${PROJECT_NAME}Targets NAMESPACE kwc:: DESTINATION lib/cmake/)

if(BUILD_TESTING)
  include(GoogleTest)
  add_executable(kwc_utils_test
    base_64_test.cc)

  target_link_libraries(kwc_utils_test
    PRIVATE kwc::strings kwc::utils GTest::gtest_main GTest::gtest)
  gtest_discover_tests(kwc_utils_test)
endif()
