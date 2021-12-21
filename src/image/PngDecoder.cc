// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "image/PngDecoder.h"

#include <png.h>
#include <setjmp.h>

#include <algorithm>
#include <cstring>
#include <string>
#include <vector>

#include "base/Check.h"
#include "base/Error.h"
#include "base/IntegralTypes.h"
#include "base/Logging.h"
#include "base/Macros.h"
#include "file/FileUtils.h"
#include "image/Image.h"

// Byte order defines provided by gcc. MSVC doesn't define those so
// we define them here.
// We assume that all windows platform out there are little endian.
#if defined(_MSC_VER) && !defined(__clang__)
    #define __ORDER_LITTLE_ENDIAN__ 0x4d2
    #define __ORDER_BIG_ENDIAN__ 0x10e1
    #define __BYTE_ORDER__ __ORDER_LITTLE_ENDIAN__
#endif

constexpr bool kLittleEndian = __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__;

#define PTR_INC(type, ptr, del) \
    (ptr = reinterpret_cast<type*>(reinterpret_cast<char*>(ptr) + (del)))
#define CPTR_INC(type, ptr, del) \
    (ptr = reinterpret_cast<const type*>(reinterpret_cast<const char*>(ptr) + (del)))

namespace kwc {
namespace image {
class Image;

namespace {
struct PNGContext {
    PNGContext() : png_ptr(nullptr), info_ptr(nullptr) {}
    ~PNGContext() {
        if (png_ptr) {
            png_destroy_read_struct(&png_ptr, info_ptr ? &info_ptr : nullptr, nullptr);
            png_ptr = nullptr;
            info_ptr = nullptr;
        }
    }
    const uint8* data;
    int data_left;
    png_structp png_ptr;
    png_infop info_ptr;
    png_uint_32 width, height;
    int num_passes;
    int color_type;
    int bit_depth;
    int channels;
    bool synthesize_to_16;
    bool error_condition;
};

void ConvertByteToWord(const uint8* p8,
                       int num_comps,
                       int p8_row_bytes,
                       int width,
                       int height_in,
                       uint16* p16,
                       int p16_row_bytes) {
    auto height = static_cast<int64>(height_in);

    // Adjust pointers to copy backwards
    width *= num_comps;
    CPTR_INC(uint8, p8, (height - 1) * p8_row_bytes + (width - 1) * sizeof(*p8));
    PTR_INC(uint16, p16, (height - 1) * p16_row_bytes + (width - 1) * sizeof(*p16));
    int bump8 = width * sizeof(*p8) - p8_row_bytes;
    int bump16 = width * sizeof(*p16) - p16_row_bytes;
    for (; height-- != 0; CPTR_INC(uint8, p8, bump8), PTR_INC(uint16, p16, bump16)) {
        for (int w = width; w-- != 0; --p8, --p16) {
            uint32 pix = *p8;
            pix |= pix << 8;
            *p16 = static_cast<uint16>(pix);
        }
    }
}

void ErrorFunc(png_structp png_ptr, png_const_charp msg) {
    auto* const ctx = reinterpret_cast<PNGContext*>(png_get_io_ptr(png_ptr));
    ctx->error_condition = true;
    longjmp(png_jmpbuf(png_ptr), 1);
}

void WarnFunc(png_structp png_ptr, png_const_charp msg) {
    LOGGING(base::WARNING) << "PNG warning: " << msg << "\n";
}

template <typename T>
void StringWriter(png_structp png_ptr, png_bytep data, png_size_t length) {
    T* const s = reinterpret_cast<T*>(png_get_io_ptr(png_ptr));
    s->append(reinterpret_cast<const char*>(data), length);
}

void StringWriterFlush(png_structp png_ptr) {}

bool DecodeInit(std::vector<uint8> data, int num_channels, int channel_depth, PNGContext* context) {
    KWC_CHECK(channel_depth == 8 || channel_depth == 16) << "Channel depth: " << channel_depth;
    KWC_CHECK(0 <= num_channels && num_channels <= 4) << "Num channels: " << num_channels;
    context->error_condition = false;
    context->channels = num_channels;
    context->png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, context, ErrorFunc, WarnFunc);

    if (!context->png_ptr) {
        LOGGING(base::ERROR) << "PNG decoding png_create_read_struct failed";
        return false;
    }

    if (setjmp(png_jmpbuf(context->png_ptr))) {
        LOGGING(base::ERROR) << "PNG decoding error trapped";
        return false;
    }

    context->info_ptr = png_create_info_struct(context->png_ptr);
    if (!context->info_ptr || context->error_condition) {
        LOGGING(base::ERROR) << "PNG decoding png_create_read_struct failed";
        return false;
    }

    context->data = reinterpret_cast<const uint8*>(&data[0]);
    context->data_left = data.size();
    png_set_read_fn(context->png_ptr, context,
                    [](png_structp png_ptr, png_bytep data, png_size_t length) {
                        auto* const ctx = reinterpret_cast<PNGContext*>(png_get_io_ptr(png_ptr));
                        if (static_cast<png_size_t>(ctx->data_left) < length) {
                            png_error(png_ptr, "More bytes requested to read than available");
                        } else {
                            std::memcpy(data, ctx->data, length);
                            ctx->data += length;
                            ctx->data_left -= length;
                        }
                    });
    png_read_info(context->png_ptr, context->info_ptr);
    png_get_IHDR(context->png_ptr, context->info_ptr, &context->width, &context->height,
                 &context->bit_depth, &context->color_type, nullptr, nullptr, nullptr);
    if (context->error_condition) {
        LOGGING(base::ERROR) << "PNG decoder error during header parsing";
        return false;
    }

    if (context->width <= 0 || context->height <= 0) {
        LOGGING(base::ERROR) << "PNG decoder invalid dimensions";
        return false;
    }

    const bool has_tRNS = (png_get_valid(context->png_ptr, context->info_ptr, PNG_INFO_tRNS)) != 0;
    if (context->channels == 0) {
        if (context->color_type == PNG_COLOR_TYPE_PALETTE) {
            context->channels = has_tRNS ? 4 : 3;
        } else {
            context->channels = png_get_channels(context->png_ptr, context->info_ptr);
        }
    }
    const bool has_alpha = (context->color_type & PNG_COLOR_MASK_ALPHA) != 0;
    if ((context->channels & 1) == 0) {
        if (has_tRNS) {
            png_set_tRNS_to_alpha(context->png_ptr);
        } else {
            png_set_add_alpha(context->png_ptr, (1 << context->bit_depth) - 1, PNG_FILLER_AFTER);
        }
    } else {
        if (has_alpha || has_tRNS) {
            png_set_strip_alpha(context->png_ptr);
        }
    }

    // If we only wanted 8 bits but are given 16, we strip off the LS 8 bits
    if (context->bit_depth > 8 && channel_depth <= 8) {
        png_set_strip_16(context->png_ptr);
    }
    context->synthesize_to_16 = (context->bit_depth <= 8 && channel_depth == 16);

    png_set_packing(context->png_ptr);
    context->num_passes = png_set_interlace_handling(context->png_ptr);

    if (channel_depth > 8 && kLittleEndian) {
        png_set_swap(context->png_ptr);
    }

    // Convert palette to RGB(A) if needed
    if (context->color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_palette_to_rgb(context->png_ptr);
    }

    // Handle grayscale images for src or dest
    const bool wants_gray = (context->channels < 3);
    const bool is_gray = !(context->color_type & PNG_COLOR_MASK_COLOR);
    if (is_gray) {
        if (context->bit_depth < 8) {
            png_set_expand_gray_1_2_4_to_8(context->png_ptr);
        }
    }
    if (wants_gray) {
        if (!is_gray) {
            png_set_rgb_to_gray(context->png_ptr, 1, 0.299, 0.587);
        }
    } else {
        if (is_gray) {
            png_set_gray_to_rgb(context->png_ptr);
        }
    }

    // Must come last to incorporate all requested transformations
    png_read_update_info(context->png_ptr, context->info_ptr);
    return true;
}

bool DecodeFinish(png_bytep data, int row_bytes, PNGContext* context) {
    KWC_CHECK(data != nullptr);

    // Need to reset the jump pointer so that we trap errors here instead of DecodeInit
    if (setjmp(png_jmpbuf(context->png_ptr))) {
        LOGGING(base::ERROR) << "PNG decoder error trapped";
        return false;
    }

    // png_read_row() takes care of offsetting the pointer based on interlacing
    for (int p = 0; p < context->num_passes; ++p) {
        png_bytep row = data;
        for (int h = context->height; h-- != 0; row += row_bytes) {
            png_read_row(context->png_ptr, row, nullptr);
        }
    }

    // Marks iDAT as valid
    png_set_rows(context->png_ptr, context->info_ptr,
                 png_get_rows(context->png_ptr, context->info_ptr));
    png_read_end(context->png_ptr, context->info_ptr);

    // Synthesize 16 bits from 8 if requested
    if (context->synthesize_to_16) {
        ConvertByteToWord(reinterpret_cast<uint8*>(data), context->channels, row_bytes,
                          context->width, context->height, reinterpret_cast<uint16*>(data),
                          row_bytes);
    }

    return !context->error_condition;
}

}  // namespace

ResultOrError<Image> ReadPNG(const std::vector<uint8>& data, ColorMode mode) {
    PNGContext context;
    // TODO(kai): Use mode
    if (!DecodeInit(data, 3, 8, &context)) {
        return KWC_INTERNAL_ERROR("Error PNG decode initialization");
    }

    Image img(context.width, context.height, context.channels);
    char* image_buffer = new char[3 * context.width * context.height];
    if (!DecodeFinish(reinterpret_cast<png_byte*>(image_buffer), 3 * context.width, &context)) {
        return KWC_INTERNAL_ERROR("Error PNG decode finalization");
    }

    auto& buffer = img.buffer();
    std::copy(image_buffer, image_buffer + 3 * context.width * context.height,
              std::back_inserter(buffer));
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

    auto buffer = &image.buffer()[0];
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
