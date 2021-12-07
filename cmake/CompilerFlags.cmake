# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

option(EMIT_PERF_DIAGNOSTICS "Enable performance diagnostics" OFF)

# Common compiler warnings used throughout the codebase
set(MSVC_WARNINGS_AND_OPTIONS
  /DWIN32            # Preprocessor flag
  /D_WINDOWS         # Preprocessor flag
  /W3                # Reasonable baseline warnings
  /MP${CPU_COUNT}    # Build with multiple processes
  /GR                # Enable RTTI
  /wd4068            # Prevents warnings on unknown pragmas
  /EHsc              # Specify exception handling model
  /Oy                # Omit frame pointer (stack trace)
  /Ot                # Favor fast code over small code
  /Qpar              # Enable auto parallelization (auto vectorization)
  /D_VARIADIC_MAX=10 # Restrict the number of variadic macros
  /DNOMINMAX         # Undefine Windows MIN/MAX macro implementation
  /bigobj            # Allow to handle large object files
)

set(CLANG_WARNINGS
  $<$<BOOL:${EMIT_PERF_DIAGNOSTICS}>:-g> # Emit debug symbols
  -Wall              # Enable all default warnings
  -Wpedantic         # Enforce strict ISO C++ standard
  -Wextra            # Enable warnings not covered by -Wall (redundant move, empty body etc.)
  -Wno-deprecated    # Disable deprecation warnings
  -Wno-deprecated-declarations # Disable deprecated declaration warnings
  -Wno-parentheses   # Disable missing parentheses warnings
  -Wno-c99-extensions   # Allow compound literals
  $<$<COMPILE_LANGUAGE:CXX>:-Wsign-compare>  # Warning when comparing integers of different sign
  $<$<COMPILE_LANGUAGE:CXX>:-Wuninitialized> # Warning when accessing uninitialized data
  $<$<COMPILE_LANGUAGE:CXX>:-Wunused>        # Warning when variable are not used in scope
)

set(GCC_WARNINGS
  ${CLANG_WARNINGS}
  $<$<COMPILE_LANGUAGE:CXX>:-Wno-useless-cast>    # Warning when performing a cast to the same type
  $<$<COMPILE_LANGUAGE:CXX>:-Woverloaded-virtual> # Additional warning for dynamic dispatch
)

# Common linker flags used throughout the codebase
set(CLANG_OPTIONS
  $<$<BOOL:${EMIT_PERF_DIAGNOSTICS}>:-Rpass-analysis=loop-vectorize> # Report missed vectorization
  $<$<STREQUAL:${ARCHITECTURE},arm>:-Ofast>
)

set(GCC_OPTIONS
  ${CLANG_OPTIONS}
  $<$<BOOL:${EMIT_PERF_DIAGNOSTICS}>:-fopt-info-vec-all> # Report missed vectorization
)

# Link this interface to set common compiler flags
add_library(kwc_compiler_flags INTERFACE)
add_library(kwc::compiler_flags ALIAS kwc_compiler_flags)

target_compile_options(kwc_compiler_flags BEFORE INTERFACE
  $<$<CXX_COMPILER_ID:MSVC>:${MSVC_WARNINGS_AND_OPTIONS}>
  $<$<CXX_COMPILER_ID:Intel>:${INTEL_WARNINGS_AND_OPTIONS}>
  $<$<CXX_COMPILER_ID:AppleClang>:${CLANG_WARNINGS}>
  $<$<CXX_COMPILER_ID:AppleClang>:${CLANG_OPTIONS}>
  $<$<CXX_COMPILER_ID:Clang>:${CLANG_WARNINGS}>
  $<$<CXX_COMPILER_ID:Clang>:${CLANG_OPTIONS}>
  $<$<CXX_COMPILER_ID:GNU>:${GCC_OPTIONS}>
  $<$<CXX_COMPILER_ID:GNU>:${GCC_WARNINGS}>)

target_link_options(kwc_compiler_flags INTERFACE
  $<$<CXX_COMPILER_ID:MSVC>:/incremental>
  $<$<CXX_COMPILER_ID:MSVC>:/debug:fastlink>
  $<$<CXX_COMPILER_ID:AppleClang>:${CLANG_OPTIONS}>
  $<$<CXX_COMPILER_ID:Clang>:${CLANG_OPTIONS}>
  $<$<CXX_COMPILER_ID:GNU>:${GCC_OPTIONS}>)

# Link this library to all build targets added later
link_libraries(kwc::compiler_flags)
install(TARGETS kwc_compiler_flags EXPORT ${PROJECT_NAME}Targets)
