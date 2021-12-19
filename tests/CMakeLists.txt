# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

include(GoogleTest)

add_executable(kwc_unittests
  UnittestsMain.cc
  unittests/AddressIpv4Test.cc
  unittests/AddressIpv6Test.cc
  unittests/AddressTest.cc
  unittests/AlignedAllocTest.cc
  unittests/AppTest.cc
  unittests/ArrayCopyTest.cc
  unittests/ArraySizeTest.cc
  unittests/Base64Test.cc
  unittests/CallbackTest.cc
  unittests/CheckedIntegerTest.cc
  unittests/CmdlineFlagsTest.cc
  unittests/CmdlineTest.cc
  unittests/DataReaderTest.cc
  unittests/DataWriterTest.cc
  unittests/EnvironmentTest.cc
  unittests/FilePathTest.cc
  unittests/HttpTransactionTest.cc
  unittests/LevenshteinTest.cc
  unittests/RefCountTest.cc
  unittests/RegexTest.cc
  unittests/ResultTest.cc
  unittests/ScopeGuardTest.cc
  unittests/StringSplitTest.cc
  unittests/StringSwitchTest.cc
  unittests/SystemInfoTest.cc
  unittests/ThreadTest.cc
  unittests/ZipTest.cc)

if(NOT MSVC)
  target_sources(kwc_unittests PUBLIC
    unittests/FileEnumeratorTest.cc
    unittests/FileTest.cc
    unittests/ForEachArgumentTest.cc
    unittests/UtilsTest.cc)
endif()

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
    GTest::gtest GTest::gmock)

set(assets_path ${CMAKE_CURRENT_SOURCE_DIR}/test_data)
configure_file(Assets.h.in ${CMAKE_CURRENT_BINARY_DIR}/Assets.h @ONLY)

add_executable(kwc_integrationtests
  IntegrationtestsMain.cc
  integrationtests/DecodePngImagesTest.cc)

target_include_directories(kwc_integrationtests
  PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${KWCToolkit_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${KWCToolkit_BINARY_DIR}/src>)

target_link_libraries(kwc_integrationtests
  PRIVATE
    kwc::base
    kwc::strings
    kwc::file
    kwc::image
    GTest::gtest)

gtest_discover_tests(kwc_unittests)
gtest_discover_tests(kwc_integrationtests)
