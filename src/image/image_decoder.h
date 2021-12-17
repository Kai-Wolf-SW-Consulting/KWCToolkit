// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_IMAGE_IMAGE_DECODER_H_
#define KWCTOOLKIT_IMAGE_IMAGE_DECODER_H_

#include <optional>
#include <vector>

#include "base/integral_types.h"
#include "image/image.h"
#include "image/metadata.h"

namespace kwc {
namespace image {

// Standalone library to load PNG/JPG/TIFF input images

enum InputImageFormat { PNG_FORMAT = 0, JPEG_FORMAT, TIFF_FORMAT, PNM_FORMAT, UNSUPPORTED_FORMAT };

// Try to determine image format by looking at the first few bytes (at least 12) and returns
// UNSUPPORTED_FORMAT if the format cannot be guessed correctly
InputImageFormat GuessImageType(const std::vector<uint8>& data);

// Interface for common image reading functions
using ImageReader = ResultOrError<Image> (*)(const std::vector<uint8>& data, ColorMode mode);

// Return the reader associated to a given file format
ImageReader GetImageReader(InputImageFormat format);

// This function is similar to GuessImageType, but returns a suitable reader function.
// The returned reader is never NULL, but unsupported formats will return an always
// failing valid reader
ImageReader GuessImageReader(const std::vector<uint8>& data);

}  // namespace image
}  // namespace kwc

#endif  // KWCTOOLKIT_IMAGE_IMAGE_DECODER_H_
