// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "image/Image.h"

#include <algorithm>

namespace kwc {
namespace image {

Image::Image(uint32 width, uint32 height, uint32 num_channels)
    : width_(width),
      height_(height),
      num_channels_(num_channels),
      row_bytes_(num_channels * width) {}

Image::Image(const uint32 width,
             const uint32 height,
             const uint32 num_channels,
             const float* data,
             long data_len)
    : width_(width),
      height_(height),
      num_channels_(num_channels),
      row_bytes_(num_channels * width) {
    std::copy(data, data + data_len, std::back_inserter(buffer_));
}

uint32 Image::width() const {
    return width_;
}

uint32 Image::height() const {
    return height_;
}

uint32 Image::numChannels() const {
    return num_channels_;
}

uint32 Image::rowBytes() const {
    return row_bytes_;
}

std::vector<uint8>& Image::buffer() {
    return buffer_;
}

}  // namespace image
}  // namespace kwc