# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

add_library(kwc_utils
  Base64.cc
  Base64.h
  Benchmark.h
  ColorPrint.cc
  ColorPrint.h
  Levenshtein.h
  Regex.cc
  Regex.h
  Zip.h)

add_library(kwc::utils ALIAS kwc_utils)

target_include_directories(kwc_utils
  PUBLIC
    $<BUILD_INTERFACE:${KWCToolkit_SOURCE_DIR}>
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
  target_sources(kwc_unittests PUBLIC
    Base64Test.cc
    LevenshteinTest.cc
    RegexTest.cc
    ZipTest.cc)
endif()