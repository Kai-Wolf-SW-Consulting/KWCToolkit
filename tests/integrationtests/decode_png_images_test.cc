// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include <gtest/gtest.h>

#include <vector>

#include "assets.h"
#include "base/integral_types.h"
#include "file/file_path.h"
#include "file/file_utils.h"
#include "image/image.h"
#include "image/image_decoder.h"

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
