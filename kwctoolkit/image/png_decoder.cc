// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/image/png_decoder.h"

#include <png.h>

#include <algorithm>
#include <csetjmp>
#include <cstring>
#include <string>
#include <vector>

#include "kwctoolkit/base/check.h"
#include "kwctoolkit/base/error.h"
#include "kwctoolkit/base/integral_types.h"
#include "kwctoolkit/base/logging.h"
#include "kwctoolkit/base/macros.h"
#include "kwctoolkit/base/scope_guard.h"
#include "kwctoolkit/file/file_utils.h"
#include "kwctoolkit/image/image.h"
#include "kwctoolkit/image/image_utils.h"

// Byte order defines provided by gcc. MSVC doesn't define those, hence we define them here.
// We assume that all Windows platform out there are little endian.
#if defined(_MSC_VER) && !defined(__clang__)
    #define __ORDER_LITTLE_ENDIAN__ 0x4d2
    #define __ORDER_BIG_ENDIAN__ 0x10e1
    #define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
#endif

constexpr bool kLittleEndian = __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__;

namespace kwc {
namespace image {
class Image;

namespace {

struct PNGContext {
    std::vector<uint8> data;
    png_size_t offset;
};

void ErrorFunc(png_structp png_ptr, png_const_charp msg) {
    LOGGING(base::ERROR) << "PNG error: " << msg << "\n";
    longjmp(png_jmpbuf(png_ptr), 1);
}

void WarnFunc(png_structp png_ptr, png_const_charp msg) {
    KWC_UNUSED(png_ptr);
    LOGGING(base::WARNING) << "PNG warning: " << msg << "\n";
}

png_voidp MallocFunc(png_structp png_ptr, png_size_t size) {
    KWC_UNUSED(png_ptr);
    if (size != (size_t)size || !CheckSizeArgsOverflow(size, 1)) {
        return nullptr;
    }
    return (png_voidp)std::malloc((size_t)size);
}

void ReadFunc(png_structp png_ptr, png_bytep data, png_size_t length) {
    auto* const ctx = (PNGContext*)png_get_io_ptr(png_ptr);
    if (ctx->data.size() - ctx->offset < length) {
        png_error(png_ptr, "ReadFunc: invalid read length (overflow)!");
    }
    std::memcpy(data, &ctx->data[ctx->offset], length);
    ctx->offset += length;
}

void FreeFunc(png_structp png_ptr, png_voidp ptr) {
    KWC_UNUSED(png_ptr);
    std::free(ptr);
}

template <typename T>
void StringWriter(png_structp png_ptr, png_bytep data, png_size_t length) {
    T* const s = reinterpret_cast<T*>(png_get_io_ptr(png_ptr));
    s->append(reinterpret_cast<const char*>(data), length);
}

void StringWriterFlush(png_structp png_ptr) {}

}  // namespace

ResultOrError<Image> ReadPNG(const std::vector<uint8>& in_data, ColorMode mode) {
    if (in_data.size() == 0) {
        return KWC_INTERNAL_ERROR("PNG error: Input data is zero length");
    }

    png_structp png = png_create_read_struct_2(PNG_LIBPNG_VER_STRING, nullptr, ErrorFunc, WarnFunc,
                                               nullptr, MallocFunc, FreeFunc);
    if (png == nullptr) {
        return KWC_INTERNAL_ERROR("PNG error: Could not initialize png info struct");
    }

    if (setjmp(png_jmpbuf(png))) {
        return KWC_INTERNAL_ERROR("PNG error trapped");
    }

    png_infop info = png_create_info_struct(png);
    if (info == nullptr) {
        return KWC_INTERNAL_ERROR("PNG error: Could not init png info struct");
    }
    png_infop end_info = png_create_info_struct(png);
    if (end_info == nullptr) {
        return KWC_INTERNAL_ERROR("PNG error: Could not init png end info struct");
    }

    PNGContext context{in_data, 0};
    png_set_read_fn(png, &context, ReadFunc);
    png_read_info(png, info);

    int color_type, bit_depth, interlaced;
    png_uint_32 width, height;
    if (!png_get_IHDR(png, info, &width, &height, &bit_depth, &color_type, &interlaced, nullptr,
                      nullptr)) {
        return KWC_INTERNAL_ERROR("PNG error: Could not get IHDR info");
    }

    png_set_strip_16(png);
    png_set_packing(png);
    if (color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(png);
    }
    if (color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
        if (bit_depth < 8) {
            png_set_expand_gray_1_2_4_to_8(png);
        }
        png_set_gray_to_rgb(png);
    }

    int has_alpha;
    if (png_get_valid(png, info, PNG_INFO_tRNS)) {
        png_set_tRNS_to_alpha(png);
        has_alpha = 1;
    } else {
        has_alpha = !!(color_type & PNG_COLOR_MASK_ALPHA);
    }

    {
        double image_gamma = 1 / 2.2, screen_gamma = 2.2;
        int srgb_intent;
        if (png_get_sRGB(png, info, &srgb_intent) || png_get_gAMA(png, info, &image_gamma)) {
            png_set_gamma(png, screen_gamma, image_gamma);
        }
    }

    bool keep_alpha = mode & (MODE_RGBA | MODE_BGRA | MODE_ARGB | MODE_RGBA_444 | MODE_YUVA);
    if (!keep_alpha) {
        png_set_strip_alpha(png);
        has_alpha = 0;
    }

    int num_passes = png_set_interlace_handling(png);
    png_read_update_info(png, info);

    uint8_t* rgb;
    int64 stride = (int64)(has_alpha ? 4 : 3) * width * sizeof(*rgb);
    if (stride != (int)stride || !CheckSizeArgsOverflow(stride, height)) {
        return KWC_INTERNAL_ERROR("PNG error: Corrupt stride or image height");
    }

    rgb = (uint8_t*)std::malloc((size_t)stride * height);
    if (rgb == nullptr) {
        return KWC_INTERNAL_ERROR("PNG error: Could not allocate memory");
    }
    KWC_SCOPE_EXIT { std::free(rgb); };

    for (int p = 0; p < num_passes; ++p) {
        png_bytep row = rgb;
        for (png_uint_32 y = 0; y < height; ++y) {
            png_read_rows(png, &row, nullptr, 1);
            row += stride;
        }
    }
    png_read_end(png, end_info);

    Image img(width, height, 3);
    auto& buffer = img.buffer();
    std::copy(rgb, rgb + 3 * width * height, std::back_inserter(buffer));

    png_destroy_read_struct((png_structpp)&png, (png_infopp)&info, (png_infopp)&end_info);
    return img;
}

bool WritePNG(const file::FilePath& filename, Image& image) {
    if (image.width() == 0 || image.height() == 0) {
        return false;
    }

    std::string png_string;
    png_infop info_ptr = nullptr;
    png_structp png_ptr =
        png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, ErrorFunc, WarnFunc);

    if (png_ptr == nullptr) {
        return false;
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        png_destroy_write_struct(&png_ptr, info_ptr ? &info_ptr : nullptr);
        return false;
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (info_ptr == nullptr) {
        png_destroy_write_struct(&png_ptr, nullptr);
        return false;
    }

    int color_type = -1;
    switch (image.numChannels()) {
        case 1: color_type = PNG_COLOR_TYPE_GRAY; break;
        case 2: color_type = PNG_COLOR_TYPE_GRAY_ALPHA; break;
        case 3: color_type = PNG_COLOR_TYPE_RGB; break;
        case 4: color_type = PNG_COLOR_TYPE_RGB_ALPHA; break;
        default: png_destroy_write_struct(&png_ptr, &info_ptr); return false;
    }

    png_set_write_fn(png_ptr, &png_string, StringWriter<std::string>, StringWriterFlush);
    png_set_compression_level(png_ptr, PNG_Z_DEFAULT_COMPRESSION);
    png_set_compression_mem_level(png_ptr, 9);
    // TODO(kai): Determine channel depth from image buffer
    const int channel_depth = 8;
    png_set_IHDR(png_ptr, info_ptr, image.width(), image.height(), channel_depth, color_type,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);

    // TODO(kai): Add writing metadata here
    png_write_info(png_ptr, info_ptr);
    if (channel_depth > 8 && kLittleEndian) {
        png_set_swap(png_ptr);
    }

    auto* buffer = &image.buffer()[0];
    auto* row = reinterpret_cast<png_byte*>(&buffer[0]);
    auto height = image.height();
    for (; height--; row += image.rowBytes()) {
        png_write_row(png_ptr, row);
    }
    png_write_end(png_ptr, nullptr);
    file::WriteFile(filename, png_string);

    png_destroy_write_struct(&png_ptr, &info_ptr);

    return true;
}

}  // namespace image
}  // namespace kwc
