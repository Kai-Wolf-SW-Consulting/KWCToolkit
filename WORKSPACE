workspace(name = "me_kaiwolf_kwctoolkit")

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

# GTest/GMock
http_archive(
    name = "com_google_googletest",
    sha256 = "353571c2440176ded91c2de6d6cd88ddd41401d14692ec1f99e35d013feda55a",
    strip_prefix = "googletest-release-1.11.0",
    urls = ["https://github.com/google/googletest/archive/release-1.11.0.zip"],
)

# Bazel platform rules.
http_archive(
    name = "platforms",
    sha256 = "b601beaf841244de5c5a50d2b2eddd34839788000fa1be4260ce6603ca0d8eb7",
    strip_prefix = "platforms-98939346da932eef0b54cf808622f5bb0928f00b",
    urls = ["https://github.com/bazelbuild/platforms/archive/98939346da932eef0b54cf808622f5bb0928f00b.zip"],
)

http_archive(
    name = "zlib",
    build_file = "//third_party:zlib.BUILD",
    sha256 = "d7510a8ee1918b7d0cad197a089c0a2cd4d6df05fee22389f67f115e738b178d",
    strip_prefix = "zlib-1.2.11",
    url = "file:third_party/zlib/1.2.11/zlib1211.zip",
)

http_archive(
    name = "libpng",
    build_file = "//third_party:libpng.BUILD",
    sha256 = "3b4b1cbd0bae6822f749d39b1ccadd6297f05e2b85a83dd2ce6ecd7d09eabdf2",
    strip_prefix = "lpng1637",
    url = "file:third_party/libpng/1.6.37/lpng1637.zip",
)

http_archive(
    name = "libjpeg",
    build_file = "//third_party:libjpeg.BUILD",
    sha256 = "bb591cf32017ef2cf83073359ec36495d4d48f27fef65c7a107813b4ac2917dc",
    url = "file:third_party/libjpeg/9e/jpeg-9e.zip",
)