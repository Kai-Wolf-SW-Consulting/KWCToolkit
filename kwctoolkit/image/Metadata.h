// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_IMAGE_METADATA_H_
#define KWCTOOLKIT_IMAGE_METADATA_H_

#include <cstddef>
#include <string>
#include <tuple>

#include <kwctoolkit/base/IntegralTypes.h>

namespace kwc {
namespace image {

using MetadataPayload = std::string;

struct Metadata {
    MetadataPayload exif;
    MetadataPayload iccp;
    MetadataPayload xmp;

    MetadataPayload& get(std::size_t index) {
        switch (index) {
            case 0: return exif;
            case 1: return iccp;
            case 2:
            default: return xmp;
        }
    }
};

#define METADATA_OFFSET(x) offsetof(Metadata, x)

}  // namespace image
}  // namespace kwc

#endif  // KWCTOOLKIT_IMAGE_METADATA_H_
