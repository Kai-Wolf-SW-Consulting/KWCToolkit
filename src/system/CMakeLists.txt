# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(kwc_system
  AlignedAlloc.cc
  AlignedAlloc.h
  Cmdline.cc
  Cmdline.h
  Cpu.cc
  Cpu.h
  Environment.cc
  Environment.h
  Executor.cc
  Executor.h
  FeatureList.cc
  FeatureList.h
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:ProcessInternalLinux.cc>
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:ProcessInternalLinux.h>
  Sleep.cc
  Sleep.h
  SystemInfo.cc
  SystemInfo.h
  SystemMemoryInfo.h
  Thread.cc
  Thread.h
  ThreadHelper.h
  ThreadTypes.cc
  ThreadTypes.h
  Timer.h)
add_library(kwc::system ALIAS kwc_system)

if(WIN32)
  target_sources(kwc_system
    PRIVATE
      SystemInfoWin.cc
      SystemMemoryInfoWin.cc
      TimerWin.cc)
elseif(APPLE)
  target_sources(kwc_system
    PRIVATE
      SystemInfoPosix.cc
    SystemInfoMac.mm
    SystemMemoryInfoMac.mm
      TimerMac.cc)
  target_link_libraries(kwc_system PRIVATE "-framework QuartzCore -framework Foundation")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  target_sources(kwc_system
    PRIVATE
      SystemInfoLinux.cc
      SystemInfoPosix.cc
      SystemMemoryInfoLinux.cc)
elseif(UNIX)
  target_sources(kwc_system
    PRIVATE
      SystemInfoPosix.cc
      TimerPosix.cc)
endif()

target_include_directories(kwc_system
  PUBLIC
  $<BUILD_INTERFACE:${KWCToolkit_SOURCE_DIR}/src>
  $<INSTALL_INTERFACE:include>)

target_link_libraries(kwc_system
  PUBLIC kwc::base kwc::strings kwc::file Threads::Threads)

install(TARGETS kwc_system
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR}
  LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR})
install(FILES ${PUBLIC_system_HEADER}
  DESTINATION ${INSTALL_INCLUDE_DIR}/kwctoolkit/system)
install(EXPORT ${PROJECT_NAME}Targets NAMESPACE kwc:: DESTINATION lib/cmake/)
