#!/bin/bash
#
# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

# Determine correct clang-format if any
if [ -x "$(command -v clang-format-13)" ]; then
  CLANG_FORMAT_EXE=clang-format-13
elif [ -x "$(command -v clang-format)" ]; then
  CLANG_FORMAT_EXE=clang-format
else
  echo "Could not find clang-format executable. Exiting gracefully."
  exit 0
fi

# Run on all affected source files
AFFECTED_FILES=$(git diff --name-only --diff-filter=AM -r origin/main | \
  grep '\(\.cc\|\.h\)$')

if [ -z "${AFFECTED_FILES}" ]; then
  exit 0
else
  ${CLANG_FORMAT_EXE} -i --style=file ${AFFECTED_FILES} 2> /dev/null
fi

# Check, if clang-format produced any diff
FORMAT_OUT=${TMPDIR:-/tmp}/clang-format-diff.out
git diff > ${FORMAT_OUT}

if [ -s ${FORMAT_OUT} ]; then
  echo "Please run clang-format -i --style=file <your changed files>"
  cat ${FORMAT_OUT}
  exit 1
fi

echo "No style issues detected"
exit 0