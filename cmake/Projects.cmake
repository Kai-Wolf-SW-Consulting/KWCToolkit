# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

unset(projects)
set(third_party_dir ${KWCToolkit_SOURCE_DIR}/third_party)

# googletest
list(APPEND projects googletest)
set(googletest_version "1.11.0")
set(googletest_path ${third_party_dir}/googletest/${googletest_version}/googletest-release-${googletest_version}.zip)
set(googletest_md5 "52943a59cefce0ae0491d4d2412c120b")
