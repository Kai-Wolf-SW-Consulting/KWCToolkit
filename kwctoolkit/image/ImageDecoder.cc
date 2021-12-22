// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/image/ImageDecoder.h"

#include <kwctoolkit/base/Error.h>
#include <kwctoolkit/base/Macros.h>
#include <kwctoolkit/image/Image.h>
#include <kwctoolkit/image/PngDecoder.h>

namespace kwc {
namespace image {
namespace {
uint32 GetBE32(const uint8 buffer[]) {
    return ((uint32)buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
}

ResultOrError<Image> FailedReader(const std::vector<uint8>&, ColorMode) {
    return KWC_UNKNOWN_FORMAT_ERROR("Format unknown");
}
}  // namespace

InputImageFormat GuessImageType(const std::vector<uint8>& data) {
    auto format = UNSUPPORTED_FORMAT;
    if (data.size() >= 12) {
        const auto magic = GetBE32(&data[0]);

        // See: https://en.wikipedia.org/wiki/List_of_file_signatures
        if (magic == 0x89504E47U) {
            format = PNG_FORMAT;
        } else if (0xFFD8FF00U <= magic && magic <= 0xFFD8FFFFU) {
            format = JPEG_FORMAT;
        } else if (magic == 0x49492A00 || magic == 0x4D4D002A) {
            format = TIFF_FORMAT;
        } else if (((magic >> 24) & 0xff) == 'P') {
            const auto type = (magic >> 16) & 0xFF;
            if (type >= '5' && type <= '7')
                format = PNM_FORMAT;
        }
    }
    return format;
}

ImageReader GetImageReader(InputImageFormat format) {
    switch (format) {
        case PNG_FORMAT: return ReadPNG;
        default: return FailedReader;
    }
}

ImageReader GuessImageReader(const std::vector<uint8>& data) {
    return GetImageReader(GuessImageType(data));
}

}  // namespace image
}  // namespace kwc