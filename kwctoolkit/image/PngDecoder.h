// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_IMAGE_PNG_DECODER_H_
#define KWCTOOLKIT_IMAGE_PNG_DECODER_H_

#include <vector>

#include "kwctoolkit/base/IntegralTypes.h"
#include "kwctoolkit/file/FilePath.h"
#include "kwctoolkit/image/Image.h"

namespace kwc {
namespace image {
class Image;

// The following PNG I/O implementation is based on libpng. This library is written in C, whose
// lack of memory safety is well documented. Besides, its error handling API is built around
// setjmp and longjmp. Furthermore, non-local gotos make static analysis pretty complicated.
// The PNG format builds on:
//
//     1. Two checksum algorithms, CRC-32 and Adler-32 that both produce 32-bit hashes but are
//        otherwise different algorithms
//     2. The DEFLATE compression format
//     3. 2-dimensional filtering. For a row of pixels it is often better (smaller) to compress
//        the residuals (i.e. the difference between pixel values and a weighted sum of their
//        neighbors above and left) than the raw values
//
// The following abstraction layer supports both 8 and 16 bit images which is superior over all
// the alternative Open-Source libraries found online.

ResultOrError<Image> ReadPNG(const std::vector<uint8>& data, ColorMode mode);

bool WritePNG(const file::FilePath& filename, Image& image);

}  // namespace image
}  // namespace kwc

#endif  // KWCTOOLKIT_IMAGE_PNG_DECODER_H_
