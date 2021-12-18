# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

add_library(kwc_net
  address.h
  address_ipv4.cc
  address_ipv4.h
  address_ipv6.cc
  address_ipv6.h)

add_library(kwc::net ALIAS kwc_net)

target_include_directories(kwc_net
  PUBLIC
    $<BUILD_INTERFACE:${KWCToolkit_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(kwc_net
  PUBLIC kwc::base)

install(TARGETS kwc_net
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR}
  LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR})
install(FILES ${PUBLIC_NET_HEADER}
  DESTINATION ${INSTALL_INCLUDE_DIR}/kwctoolkit/net)
install(EXPORT ${PROJECT_NAME}Targets NAMESPACE kwc:: DESTINATION lib/cmake/)

if(BUILD_TESTING)
  target_sources(kwc_unittests PUBLIC
    address_ipv4_test.cc
    address_ipv6_test.cc
    address_test.cc)
endif()
