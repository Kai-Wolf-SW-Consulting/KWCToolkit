// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include <Assets.h>
#include <gtest/gtest.h>

#include <vector>

#include "kwctoolkit/base/IntegralTypes.h"
#include "kwctoolkit/file/FilePath.h"
#include "kwctoolkit/file/FileUtils.h"
#include "kwctoolkit/image/Image.h"
#include "kwctoolkit/image/ImageDecoder.h"

using namespace kwc;

TEST(DecodePngTests, ReadPngImage) {
    using namespace kwc::file;

    std::vector<uint8> data;
    ASSERT_TRUE(ReadFile(FilePath(kAssetsPath).append("basket_rgb.png"), data));

    const auto reader = image::GuessImageReader(data);
    auto result = reader(data, image::MODE_RGB);
    ASSERT_FALSE(result.isError());

    const auto image = result.getSuccess();
    EXPECT_EQ(200, image.width());
    EXPECT_EQ(173, image.height());
    EXPECT_EQ(3, image.numChannels());
}
