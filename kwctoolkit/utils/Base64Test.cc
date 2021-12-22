// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/utils/Base64.h"

#include <gtest/gtest.h>

#include <vector>

#include "kwctoolkit/base/IntegralTypes.h"
#include "kwctoolkit/strings/StringUtils.h"

enum Encoding {
    // Official means the result is the expected encoding of the input
    Official,
    // Valid means the result is *one* valid encoding of the input, but
    // not necessarily the official one
    Valid,
    // Invalid means that the encoded data is Invalid
    Invalid,
};

struct Base64TestData {
    enum Encoding encoding;
    const char* decoded;
    const char* encoded;
};

static constexpr Base64TestData kTestVectors[] = {
    {Official,       "",                     ""},
    {Official,      "f",               "Zg==\n"},
    {Official,     "fo",               "Zm8=\n"},
    {Official,    "foo",               "Zm9v\n"},
    {Official,   "foob",           "Zm9vYg==\n"},
    {Official,  "fooba",           "Zm9vYmE=\n"},
    {Official, "foobar",           "Zm9vYmFy\n"},
    {   Valid, "foobar",         "Zm9vYmFy\n\n"},
    {   Valid, "foobar",        " Zm9vYmFy\n\n"},
    {   Valid, "foobar", " Z m 9 v Y m F y\n\n"},
    { Invalid,       "",          "Zm9vYmFy=\n"},
    { Invalid,       "",         "Zm9vYmFy==\n"},
    { Invalid,       "",        "Zm9vYmFy===\n"},
    { Invalid,       "",                    "Z"},
    { Invalid,       "",                  "Z\n"},
    { Invalid,       "",                 "ab!c"},
    { Invalid,       "",                 "ab=c"},
    { Invalid,       "",                  "abc"},
};

class Base64Test : public ::testing::TestWithParam<Base64TestData> {};

INSTANTIATE_TEST_SUITE_P(Basic64Tests, Base64Test, testing::ValuesIn(kTestVectors));

using namespace kwc;
using namespace kwc::utils;

TEST_P(Base64Test, Encode) {
    const auto& param = GetParam();
    if (param.encoding != Official) {
        return;
    }

    const auto len_decoded = strlen(param.decoded);
    auto* decoded = reinterpret_cast<const unsigned char*>(param.decoded);
    auto encoded = strings::TrimString(param.encoded, "\n", strings::TRIM_ALL);
    const auto result = Base64Encode(decoded, len_decoded);
    EXPECT_EQ(result, encoded);
}

TEST_P(Base64Test, Decode) {
    const auto& param = GetParam();
    if (param.encoding == Valid) {
        return;
    }

    const auto encoded = strings::TrimString(param.encoded, "\n", strings::TRIM_ALL);
    const auto result = Base64Decode(encoded);

    if (param.encoding == Invalid) {
        // TODO(kwc): Add invalid checks to implementation
    } else if (param.encoding == Official) {
        EXPECT_EQ(param.decoded, result);
    }
}
