# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

add_library(kwc_audio
  dft.h
  pcm_utils.cc
  pcm_utils.h
  sine_generator.cc
  sine_generator.h)

add_library(kwc::audio ALIAS kwc_audio)

target_include_directories(kwc_audio
  PUBLIC
    $<BUILD_INTERFACE:${KWCToolkit_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(kwc_audio
  PUBLIC kwc::base)

install(TARGETS kwc_audio
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR}
  LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR})
install(FILES ${PUBLIC_IMAGE_HEADER}
  DESTINATION ${INSTALL_INCLUDE_DIR}/kwctoolkit/audio)
install(EXPORT ${PROJECT_NAME}Targets NAMESPACE kwc:: DESTINATION lib/cmake/)

