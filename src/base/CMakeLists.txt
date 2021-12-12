# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

add_library(kwc_base
  array_copy.h
  array_size.h
  assert.cc
  assert.h
  callback.h
  callback_impl.h
  callback_types.h
  check.cc
  check.h
  cmdline.cc
  cmdline.h
  cmdline_flags.cc
  cmdline_flags.h
  compiler.h
  error.h
  error_trace.cc
  error_trace.h
  feature_list.cc
  feature_list.h
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:for_each_argument.h>
  for_each_argument.h
  integral_types.h
  logging.cc
  logging.h
  macros.h
  platform.h
  ref_count.cc
  ref_count.h
  result.cc
  result.h
  rw_protected.h
  scope_guard.h
  status.cc
  status.h
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:utils.h>)

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
    array_copy_test.cc
    array_size_test.cc
    callback_test.cc
    cmdline_flags_test.cc
    cmdline_test.cc
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:for_each_argument_test.cc>
    ref_count_test.cc
    result_test.cc
    scope_guard_test.cc
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:utils_test.cc>)

  target_link_libraries(kwc_base_test
    PRIVATE kwc::base kwc::strings kwc::file GTest::gtest_main GTest::gtest)
  gtest_discover_tests(kwc_base_test)
endif()
