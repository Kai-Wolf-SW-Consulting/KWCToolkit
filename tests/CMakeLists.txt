# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

include(GoogleTest)
find_package(GTest)

add_executable(kwc_unittests
  unittests_main.cc)

target_include_directories(kwc_unittests
  PRIVATE
    $<BUILD_INTERFACE:${KWCToolkit_SOURCE_DIR}>
    $<BUILD_INTERFACE:${KWCToolkit_BINARY_DIR}>)

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
    GTest::gtest GTest::gmock)

set(assets_path ${CMAKE_CURRENT_SOURCE_DIR}/test_data)
configure_file(assets.h.in ${CMAKE_CURRENT_BINARY_DIR}/assets.h @ONLY)

add_executable(kwc_integrationtests
  integrationtests_main.cc
  integrationtests/decode_png_images_test.cc)

target_include_directories(kwc_integrationtests
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${KWCToolkit_SOURCE_DIR}>
    $<BUILD_INTERFACE:${KWCToolkit_BINARY_DIR}>)

target_link_libraries(kwc_integrationtests
  PRIVATE
    kwc::base
    kwc::strings
    kwc::file
    kwc::image
    GTest::gtest)

gtest_discover_tests(kwc_unittests)
gtest_discover_tests(kwc_integrationtests)
