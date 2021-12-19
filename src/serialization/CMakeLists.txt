# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

add_library(kwc_serialization
  DataReader.cc
  DataReader.h
  DataWriter.cc
  DataWriter.h
  InMemoryDataReader.cc
  IstreamDataReader.cc
  StringDataWriter.cc)

add_library(kwc::serialization ALIAS kwc_serialization)

target_include_directories(kwc_serialization
  PUBLIC
    $<BUILD_INTERFACE:${KWCToolkit_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(kwc_serialization
  PUBLIC kwc::base)

install(TARGETS kwc_serialization
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR}
  LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR})
install(FILES ${PUBLIC_SERIALIZATION_HEADER}
  DESTINATION ${INSTALL_INCLUDE_DIR}/kwctoolkit/serialization)
install(EXPORT ${PROJECT_NAME}Targets NAMESPACE kwc:: DESTINATION lib/cmake/)

if(BUILD_TESTING)
  target_sources(kwc_unittests PUBLIC
    DataReaderTest.cc
    DataWriterTest.cc)
endif()
