# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

add_library(kwc_file
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:File.cc>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:File.h>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:FileEnumerator.cc>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:FileEnumerator.h>
  FileInfo.cc
  FileInfo.h
  FilePath.cc
  FilePath.h
  FilePathConstants.h
  FileUtils.cc
  FileUtils.h)

add_library(kwc::file ALIAS kwc_file)

target_include_directories(kwc_file
  PUBLIC
  $<BUILD_INTERFACE:${KWCToolkit_SOURCE_DIR}>
  $<INSTALL_INTERFACE:include>)

target_link_libraries(kwc_file
  PUBLIC kwc::base)

install(TARGETS kwc_file
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR}
  LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR})
install(FILES ${PUBLIC_file_HEADER}
  DESTINATION ${INSTALL_INCLUDE_DIR}/kwctoolkit/file)
install(EXPORT ${PROJECT_NAME}Targets NAMESPACE kwc:: DESTINATION lib/cmake/)

if(BUILD_TESTING)
  target_sources(kwc_unittests PUBLIC
    FilePathTest.cc)
  if(NOT MSVC)
    target_sources(kwc_unittests PUBLIC
      FileEnumeratorTest.cc
      FileTest.cc)
  endif()
endif()