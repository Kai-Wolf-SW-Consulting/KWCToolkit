# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

configure_file(AppInfo.h.in ${CMAKE_CURRENT_BINARY_DIR}/AppInfo.h)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_library(kwc_app
  App.cc
  App.h)
add_library(kwc::app ALIAS kwc_app)

target_link_libraries(kwc_app
  PUBLIC kwc::base kwc::file kwc::strings kwc::system)

add_executable(kwc_main Main.cc)
target_link_libraries(kwc_main PRIVATE kwc::app)

install(TARGETS kwc_app
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR}
  LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR})
install(EXPORT ${PROJECT_NAME}Targets NAMESPACE kwc:: DESTINATION lib/cmake/)

if(BUILD_TESTING)
  target_sources(kwc_unittests PUBLIC
    AppTest.cc)
endif()
