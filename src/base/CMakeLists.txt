# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

configure_file(BuildFlagsInternal.h.in ${CMAKE_CURRENT_BINARY_DIR}/BuildFlagsInternal.h)

add_library(kwc_base
  ArrayCopy.h
  ArraySize.h
  Assert.cc
  Assert.h
  Callback.h
  CallbackImpl.h
  CallbackTypes.h
  Check.cc
  Check.h
  CheckedInteger.h
  CmdlineFlags.cc
  CmdlineFlags.h
  Compiler.h
  Error.h
  ErrorTrace.cc
  ErrorTrace.h
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:ForEachArgument.h>
  ForEachArgument.h
  IntegralTypes.h
  Logging.cc
  Logging.h
  Macros.h
  Platform.h
  RefCount.cc
  RefCount.h
  Result.cc
  Result.h
  RwProtected.h
  ScopeGuard.h
  Status.cc
  Status.h
  $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:Utils.h>)

add_library(kwc::base ALIAS kwc_base)

target_include_directories(kwc_base
  PUBLIC
    $<BUILD_INTERFACE:${KWCToolkit_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${KWCToolkit_BINARY_DIR}/src>
  $<INSTALL_INTERFACE:include>)

install(TARGETS kwc_base
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR}
  LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR})
install(FILES ${PUBLIC_BASE_HEADER}
  DESTINATION ${INSTALL_INCLUDE_DIR}/kwctoolkit/base)
install(EXPORT ${PROJECT_NAME}Targets NAMESPACE kwc:: DESTINATION lib/cmake/)

if(BUILD_TESTING)
  target_sources(kwc_unittests PUBLIC
    ArrayCopyTest.cc
    ArraySizeTest.cc
    CallbackTest.cc
    CheckedIntegerTest.cc
    CmdlineFlagsTest.cc
    RefCountTest.cc
    ResultTest.cc
    ScopeGuardTest.cc)
  if(NOT MSVC)
    target_sources(kwc_unittests PUBLIC
      ForEachArgumentTest.cc
      UtilsTest.cc)
  endif()
endif()
