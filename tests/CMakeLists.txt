# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

include(GoogleTest)
add_executable(kwc_unittests
  unittests_main.cc)

target_include_directories(kwc_unittests
  PRIVATE
    $<BUILD_INTERFACE:${KWCToolkit_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${KWCToolkit_BINARY_DIR}/src>)

target_link_libraries(kwc_unittests
  PRIVATE
    kwc::app
    kwc::base
    kwc::strings
    kwc::file
    kwc::net
    kwc::serialization
    kwc::system
    kwc::transport
    kwc::utils
    GTest::gtest_main GTest::gtest GTest::gmock)

gtest_discover_tests(kwc_unittests)
