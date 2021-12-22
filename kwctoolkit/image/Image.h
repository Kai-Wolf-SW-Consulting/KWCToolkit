// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_IMAGE_IMAGE_H_
#define KWCTOOLKIT_IMAGE_IMAGE_H_

#include <memory>
#include <variant>
#include <vector>

#include "kwctoolkit/base/ErrorTrace.h"
#include "kwctoolkit/base/IntegralTypes.h"
#include "kwctoolkit/base/Macros.h"
#include "kwctoolkit/base/Result.h"

namespace kwc {
namespace image {

// These modes describe the byte-ordering of packed samples in memory.
// For instance, MODE_RGBA relates to samples ordered as R,G,B,A,R,G,B,A, etc.
enum ColorMode {
    MODE_RGB = 0,
    MODE_RGBA = 1,
    MODE_BGR = 2,
    MODE_BGRA = 3,
    MODE_ARGB = 4,
    MODE_RGBA_444 = 5,
    MODE_RGB_565 = 6,
    MODE_YUV = 7,
    MODE_YUVA = 8,
    MODE_LAST = 9
};

// These specify the desired output format for a given image buffer, except YUV and YUVA formats
enum OutputFileFormat { FORMAT_PNG = 0, FORMAT_PPM, FORMAT_BMP, FORMAT_TIFF };

// The following types are meant to be used as return values for functions that are expected to,
// but might fail.
using MaybeError = base::Result<void, base::ErrorTrace>;

template <typename T>
using ResultOrError = base::Result<T, base::ErrorTrace>;

// The following aspects for the interface of this class are of importance:
//
//   + Backend must support either RGB(A) or YUV(A) backings
//   + Hide implementation details for ABI compatibility
//   + Get/Set specific image encodings, e.g. RGBA_4444, BGR, etc.
//   + Access to common properties such as width, height, channels, step etc.
//   + Allow access to *raw* underlying data structures (e.g. rgba buffer, yuv buffer)
//   + Provide pixel access methods
//       - inline T* operator[](const int row) { return data_ + row * stride; }
//       - inline const T* operator[](const int row) const { return data_ + row * stride; }
//   + Provide typical methods that are to be expected
//       - clear(), setPixel()/getPixel(), crop(), rescale()
//   + Has optimizations for different target architectures:
//       - image_neon, image_avx2
//   + Works nicely with I/O methods (but must not be part of this class)
//       - Decoding/Encoding from/into PNG, TIFF, BMP, JPG, etc.
//
class Image {
  public:
    Image() = default;
    Image(uint32 width, uint32 height, uint32 num_channels);
    Image(uint32 width, uint32 height, uint32 num_channels, const float* data, long data_len);
    ~Image() = default;

    KWC_NO_DISCARD uint32 width() const;
    KWC_NO_DISCARD uint32 height() const;
    KWC_NO_DISCARD uint32 numChannels() const;
    KWC_NO_DISCARD uint32 rowBytes() const;

    std::vector<uint8>& buffer();

  private:
    uint32 width_, height_;
    uint32 num_channels_;
    uint32 row_bytes_;  // num columns x element size (+ optional padding)
    std::vector<uint8> buffer_;
};

}  // namespace image
}  // namespace kwc

#endif  // KWCTOOLKIT_IMAGE_IMAGE_H_
