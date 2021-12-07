# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

if(ENABLE_CPPCHECK)
  find_program(CPPCHECK_EXE cppcheck)
  if(CPPCHECK_EXE)
    set(CMAKE_CXX_CPPCHECK
      ${CPPCHECK_EXE} --suppress=missingInclude --enable=all --inline-suppr --inconclusive)
  else()
    message(SEND_ERROR "cppcheck requested but not found")
  endif()
endif()

if(ENABLE_CLANG_TIDY)
  find_program(CLANGTIDY_EXE clang-tidy)
  if(CLANGTIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY_EXE} -extra-arg=-Wno-unknown-warning-option)
  else()
    message(SEND_ERROR "clang-tidy requested but not found")
  endif()
endif()

if(ENABLE_INCLUDE_WHAT_YOU_USE)
  find_program(IWYU_EXE include-what-you-use)
  if(IWYU_EXE)
    set(CMAKE_CXX_INCLUDE_WHAT_YOU_USE ${IWYU_EXE})
  else()
    message(SEND_ERROR "include-what-you-use requested but not found")
  endif()
endif()