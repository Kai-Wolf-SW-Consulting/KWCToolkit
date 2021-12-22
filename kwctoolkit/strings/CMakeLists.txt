# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

add_library(kwc_strings
  StringSplit.cc
  StringSplit.h
  StringSwitch.h
  StringUtils.cc
  StringUtils.h
  UtfStringConversion.cc
  UtfStringConversion.h)

add_library(kwc::strings ALIAS kwc_strings)

target_include_directories(kwc_strings
  PUBLIC
    $<BUILD_INTERFACE:${KWCToolkit_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(kwc_strings
  PUBLIC kwc::base third_party::icu)

install(TARGETS kwc_strings
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR}
  LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR})
install(FILES ${PUBLIC_strings_HEADER}
  DESTINATION ${INSTALL_INCLUDE_DIR}/kwctoolkit/strings)
install(EXPORT ${PROJECT_NAME}Targets NAMESPACE kwc:: DESTINATION lib/cmake/)

if(BUILD_TESTING)
  target_sources(kwc_unittests PUBLIC
    StringSplitTest.cc
    StringSwitchTest.cc)
endif()