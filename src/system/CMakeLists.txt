# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(kwc_system
  cpu.cc
  cpu.h
  environment.cc
  environment.h
  thread.cc
  thread.h
  thread_types.cc
  thread_types.h
  timer.h)
add_library(kwc::system ALIAS kwc_system)

if(WIN32)
  target_sources(kwc_system PRIVATE timer_win.cc)
elseif(APPLE)
  target_sources(kwc_system PRIVATE timer_mac.cc)
  target_link_libraries(kwc_system PRIVATE "-framework QuartzCore")
elseif(UNIX)
  target_sources(kwc_system PRIVATE timer_posix.cc)
endif()

target_include_directories(kwc_system
  PUBLIC
  $<BUILD_INTERFACE:${KWCToolkit_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include>)

target_link_libraries(kwc_system
  PUBLIC kwc::base kwc::strings Threads::Threads)

install(TARGETS kwc_system
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR}
  LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR})
install(FILES ${PUBLIC_system_HEADER}
  DESTINATION ${INSTALL_INCLUDE_DIR}/kwctoolkit/system)
install(EXPORT ${PROJECT_NAME}Targets NAMESPACE kwc:: DESTINATION lib/cmake/)

if(BUILD_TESTING)
  include(GoogleTest)
  add_executable(kwc_system_test
    environment_test.cc
    thread_test.cc)

  target_link_libraries(kwc_system_test
    PRIVATE kwc::system GTest::gtest_main GTest::gtest)
  gtest_discover_tests(kwc_system_test)
endif()
