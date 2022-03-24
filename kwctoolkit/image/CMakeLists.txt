# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

find_package(PNG)

add_library(kwc_image
  image.cc
  image.h
  image_decoder.cc
  image_decoder.h
  metadata.cc
  metadata.h
  png_decoder.cc
  png_decoder.h)

add_library(kwc::image ALIAS kwc_image)

target_include_directories(kwc_image
  PUBLIC
    $<BUILD_INTERFACE:${KWCToolkit_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>)

target_link_libraries(kwc_image
  PUBLIC kwc::base kwc::file PNG::PNG)

install(TARGETS kwc_image
  EXPORT ${PROJECT_NAME}Targets
  ARCHIVE DESTINATION ${INSTALL_ARCHIVE_DIR}
  LIBRARY DESTINATION ${INSTALL_LIBRARY_DIR})
install(FILES ${PUBLIC_IMAGE_HEADER}
  DESTINATION ${INSTALL_INCLUDE_DIR}/kwctoolkit/image)
install(EXPORT ${PROJECT_NAME}Targets NAMESPACE kwc:: DESTINATION lib/cmake/)

