// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include <gtest/gtest.h>

#include <vector>

#include "Assets.h"
#include "kwctoolkit/base/integral_types.h"
#include "kwctoolkit/file/file_path.h"
#include "kwctoolkit/file/file_utils.h"
#include "kwctoolkit/image/image.h"
#include "kwctoolkit/image/image_decoder.h"

using namespace kwc;

TEST(DecodePngTests, ReadPngImage) {
    using namespace kwc::file;

    std::vector<uint8> data;
    EXPECT_TRUE(ReadFile(FilePath(kAssetsPath).append("basket_rgb.png"), data));

    const auto reader = image::GuessImageReader(data);
    auto result = reader(data, image::MODE_RGB);
    ASSERT_FALSE(result.isError());

    const auto image = result.getSuccess();
    EXPECT_EQ(200, image.width());
    EXPECT_EQ(173, image.height());
    EXPECT_EQ(3, image.numChannels());
}
