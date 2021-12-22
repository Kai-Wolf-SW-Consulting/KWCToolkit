// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include <iostream>
#include <string>

#include "kwctoolkit/base/IntegralTypes.h"
#include "kwctoolkit/base/Macros.h"
#include "kwctoolkit/base/Status.h"
#include "kwctoolkit/serialization/DataReader.h"

namespace kwc {
class Callback;
namespace serialization {

using base::Status;

class IstreamDataReader : public DataReader {
  public:
    IstreamDataReader(std::istream* stream, int64 total_len, Callback* delete_cb)
        : DataReader(delete_cb), stream_(stream) {
        if (total_len != -1) {
            setTotalLength(total_len);
        }

        int64 pos = stream->tellg();
        if (pos != 0) {
            setStatus(Status(base::error::INVALID_ARGUMENT, "Stream not at the beginning"));
        }

        if (stream->eof()) {
            setDone(true);
        } else if (stream->fail()) {
            setStatus(Status(base::error::INVALID_ARGUMENT, "Invalid stream"));
        }
    }

    ~IstreamDataReader() override = default;

    bool isSeekable() const override { return true; }

  protected:
    int64 doReadIntoBuffer(int64 max_bytes, char* storage) override {
        stream_->read(storage, max_bytes);

        if (stream_->rdstate() != 0u) {
            // If we were at EOF and tried to read, then it will set the fail
            // bit. Note that the stream won't necessarily know it is at EOF
            // before the read if it didn't encountered it yet.
            if (stream_->eof()) {
                setDone(true);
            } else if (stream_->fail()) {
                setStatus(Status(base::error::UNKNOWN, "Could not read stream"));
            }
        }

        return stream_->gcount();
    }

    int64 doSetOffset(int64 position) override {
        stream_->clear();
        stream_->seekg(position);
        if (stream_->fail() || stream_->bad()) {
            setStatus(Status(base::error::UNKNOWN, "Could not seek stream"));
            return -1;
        }

        return stream_->tellg();
    }

  private:
    std::istream* stream_;

    DISALLOW_COPY_AND_ASSIGN(IstreamDataReader);
};

DataReader* CreateUnmanagedIstreamDataReader(std::istream* stream) {
    return CreateManagedIstreamDataReader(stream, -1, nullptr);
}

DataReader* CreateUnmanagedIstreamDataReader(std::istream* stream, int64 length) {
    return CreateManagedIstreamDataReader(stream, length, nullptr);
}

DataReader* CreateManagedIstreamDataReader(std::istream* stream, Callback* delete_cb) {
    return CreateManagedIstreamDataReader(stream, -1, delete_cb);
}

DataReader* CreateManagedIstreamDataReader(std::istream* stream,
                                           int64 length,
                                           Callback* delete_cb) {
    return new IstreamDataReader(stream, length, delete_cb);
}

}  // namespace serialization
}  // namespace kwc