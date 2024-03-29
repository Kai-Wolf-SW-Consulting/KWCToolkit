# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

add_library(kwc_transport
  http_request.cc
  http_request.h
  http_response.cc
  http_response.h
  http_transaction.cc
  http_transaction.h
  http_types.h
  simple_http_transaction.cc
  simple_http_transaction.h)

add_library(kwc::transport ALIAS kwc_transport)

target_include_directories(kwc_transport
  PUBLIC
    $<BUILD_INTERFACE:${KWCToolkit_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(kwc_transport
  PUBLIC kwc::base kwc::strings kwc::serialization kwc::system)

install(TARGETS kwc_transport
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR}
  LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR})
install(FILES ${PUBLIC_TRANSPORT_HEADER}
  DESTINATION ${INSTALL_INCLUDE_DIR}/kwctoolkit/transport)
install(EXPORT ${PROJECT_NAME}Targets NAMESPACE kwc:: DESTINATION lib/cmake/)

if(BUILD_TESTING)
  target_sources(kwc_unittests PUBLIC
    http_transaction_test.cc)
endif()
