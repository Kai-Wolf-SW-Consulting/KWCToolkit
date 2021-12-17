# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

function(build_option option_var help_string)
  option(${option_var} ${help_string} ${ARGN})
  if(${option_var})
    set(${option_var} 1 CACHE STRING "" FORCE)
  else()
    set(${option_var} 0 CACHE STRING "" FORCE)
  endif()
endfunction()