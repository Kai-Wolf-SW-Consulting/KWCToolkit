# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

configure_file(app_info.h.in ${CMAKE_CURRENT_BINARY_DIR}/app_info.h)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(kwc_app
  app.cc
  app.h)
add_library(kwc::app ALIAS kwc_app)

target_link_libraries(kwc_app
  PUBLIC kwc::base kwc::file kwc::strings kwc::system)

target_include_directories(kwc_app
  PUBLIC
    $<BUILD_INTERFACE:${KWCToolkit_SOURCE_DIR}>
    $<BUILD_INTERFACE:${KWCToolkit_BINARY_DIR}>
    $<INSTALL_INTERFACE:include>)

add_executable(kwc_main main.cc)
target_link_libraries(kwc_main PRIVATE kwc::app)

install(TARGETS kwc_app
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR}
  LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR})
install(EXPORT ${PROJECT_NAME}Targets NAMESPACE kwc:: DESTINATION lib/cmake/)

if(BUILD_TESTING)
  target_sources(kwc_unittests PUBLIC
    app_test.cc)
endif()