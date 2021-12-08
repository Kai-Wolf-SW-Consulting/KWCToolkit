# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

add_library(kwc_file
  file.cc
  file.h
  file_path.cc
  file_path.h
  file_utils.cc
  file_utils.h)

add_library(kwc::file ALIAS kwc_file)

target_include_directories(kwc_file
  PUBLIC
  $<BUILD_INTERFACE:${KWCToolkit_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include>)

install(TARGETS kwc_file
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR}
  LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR})
install(FILES ${PUBLIC_file_HEADER}
  DESTINATION ${INSTALL_INCLUDE_DIR}/kwctoolkit/file)
install(EXPORT ${PROJECT_NAME}Targets NAMESPACE kwc:: DESTINATION lib/cmake/)

if(BUILD_TESTING)
  include(GoogleTest)
  add_executable(kwc_file_test
    file_path_test.cc)

  target_link_libraries(kwc_file_test
    PRIVATE kwc::file GTest::gtest_main GTest::gtest)
  gtest_discover_tests(kwc_file_test)
endif()
