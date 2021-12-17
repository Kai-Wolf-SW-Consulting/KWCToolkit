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

# zlib
list(APPEND projects zlib)
set(zlib_version "1.2.11")
string(REPLACE "." "" zlib_version_string ${zlib_version})
set(zlib_path ${third_party_dir}/zlib/${zlib_version}/zlib${zlib_version_string}.zip)
set(zlib_md5 "16b41357b2cd81bca5e1947238e64465")

# libpng
list(APPEND projects libpng)
set(libpng_version "1.6.37")
string(REPLACE "." "" libpng_version_string ${libpng_version})
set(libpng_path ${third_party_dir}/libpng/${libpng_version}/lpng${libpng_version_string}.zip)
set(libpng_md5 "883764a8ebb8352904679f2ec70eda69")

