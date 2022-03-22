// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/file/file_path.h"

#include <gtest/gtest.h>

#include <cstddef>

#include "kwctoolkit/base/array_size.h"

using namespace kwc::file;

class FilePathTest : public testing::Test {
  protected:
    struct UnaryTestData {
        const std::string::value_type* input;
        const std::string::value_type* expected;
    };

    struct BinaryTestData {
        const std::string::value_type* inputs[2];
        const std::string::value_type* expected;
    };

    struct BinaryBooleanTestData {
        const std::string::value_type* inputs[2];
        bool expected;
    };
};

TEST_F(FilePathTest, CheckBaseName) {
    const struct UnaryTestData cases[] = {
  // clang-format off
        { "",             "" },
        { "aa",           "aa" },
        { "/aa/bb",       "bb" },
        { "/aa/bb/",      "bb" },
        { "/aa/bb//",     "bb" },
        { "/aa/bb/ccc",   "ccc" },
        { "/aa",          "aa" },
        { "/",            "/" },
        { "//",           "//" },
        { "///",          "/" },
        { "aa/",          "aa" },
        { "aa/bb",        "bb" },
        { "aa/bb/",       "bb" },
        { "aa/bb//",      "bb" },
        { "aa//bb//",     "bb" },
        { "aa//bb/",      "bb" },
        { "aa//bb",       "bb" },
        { "//aa/bb",      "bb" },
        { "//aa/",        "aa" },
        { "//aa",         "aa" },
  // clang-format on
    };

    for (auto i = 0u; i < arraysize(cases); i++) {
        FilePath input(cases[i].input);
        FilePath observed = input.baseName();
        EXPECT_EQ(cases[i].expected, observed.value())
            << "i: " << i << ", input: " << input.value();
    }
}

TEST_F(FilePathTest, CheckDirectoryNavigations) {
    const struct UnaryTestData cases[] = {
  // clang-format off
        { "",             "."},
        { "aa",           "." },
        { "/aa/bb",       "/aa" },
        { "/aa/bb/",      "/aa" },
        { "/aa/bb//",     "/aa" },
        { "/aa/bb/ccc",   "/aa/bb" },
        { "/aa",          "/" },
        { "/aa/",         "/" },
        { "/",            "/" },
        { "//",           "//" },
        { "///",          "/" },
        { "aa/",          "." },
        { "aa/bb",        "aa" },
        { "aa/bb/",       "aa" },
        { "aa/bb//",      "aa" },
        { "aa//bb//",     "aa" },
        { "aa//bb/",      "aa" },
        { "aa//bb",       "aa" },
        { "//aa/bb",      "//aa" },
        { "//aa/",        "//" },
        { "//aa",         "//" },
  // clang-format on
    };

    for (auto i = 0u; i < arraysize(cases); i++) {
        FilePath input(cases[i].input);
        FilePath observed = input.directoryName();
        EXPECT_EQ(std::string(cases[i].expected), observed.value())
            << "i: " << i << ", input: " << input.value();
    }
}

TEST_F(FilePathTest, CheckAppendPaths) {
    const struct BinaryTestData cases[] = {
  // clang-format off
        { { "",       "cc" }, "cc" },
        { { ".",      "ff" }, "ff" },
        { { "/",      "cc" }, "/cc" },
        { { "/aa",      "" }, "/aa" },
        { { "/aa/",     "" }, "/aa" },
        { { "//aa",     "" }, "//aa" },
        { { "//aa/",    "" }, "//aa" },
        { { "//",     "aa" }, "//aa" },
  // clang-format on
    };

    for (std::size_t i = 0; i < arraysize(cases); ++i) {
        FilePath root(cases[i].inputs[0]);
        std::string leaf(cases[i].inputs[1]);
        FilePath observed = root.append(leaf);
        EXPECT_EQ(std::string(cases[i].expected), observed.value())
            << "i: " << i << ", root: " << root.value() << ", leaf: " << leaf;
        FilePath observed_path = root.append(FilePath(leaf));
        EXPECT_EQ(std::string(cases[i].expected), observed_path.value())
            << "i: " << i << ", root: " << root.value() << ", leaf: " << leaf;
    }
}

TEST_F(FilePathTest, CheckEqual) {
    const struct BinaryBooleanTestData cases[] = {
  // clang-format off
        { { "/foo_/bar/baz",  "/foo_/bar/baz" },      true },
        { { "/foo_/bar",      "/foo_/bar/baz" },      false },
        { { "/foo_/bar/baz",  "/foo_/bar" },          false },
        { { "//foo_/bar/",    "//foo_/bar/" },        true },
        { { "/foo_/bar",      "/foo2/bar" },         false },
        { { "/foo_/bar.txt",  "/foo_/bar" },          false },
        { { "foo_/bar",       "foo_/bar" },           true },
        { { "foo_/bar",       "foo_/bar/baz" },       false },
        { { "",              "foo_" },               false },
  // clang-format on
    };

    for (std::size_t i = 0; i < arraysize(cases); ++i) {
        FilePath a(cases[i].inputs[0]);
        FilePath b(cases[i].inputs[1]);
        EXPECT_EQ(a == b, cases[i].expected)
            << "equality i: " << i << ", a: " << a.value() << ", b: " << b.value();
    }

    for (std::size_t i = 0; i < arraysize(cases); ++i) {
        FilePath a(cases[i].inputs[0]);
        FilePath b(cases[i].inputs[1]);
        EXPECT_EQ(a != b, !cases[i].expected)
            << "inequality i: " << i << ", a: " << a.value() << ", b: " << b.value();
    }
}