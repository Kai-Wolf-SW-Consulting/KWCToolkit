// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "serialization/DataWriter.h"

#include <algorithm>
#include <memory>
#include <ostream>

#include "base/Logging.h"
#include "serialization/DataReader.h"

namespace kwc {
class Callback;
namespace serialization {

DataWriter::DataWriter() : size_(0), has_begun_(false) {}

void DataWriter::clear() {
    size_ = 0;
    status_ = doClear();
}

void DataWriter::begin() {
    size_ = 0;
    status_ = doBegin();
    has_begun_ = status_.ok();
}

void DataWriter::end() {
    status_ = doEnd();
}

Status DataWriter::writeData(int64 bytes, const char* data) {
    if (!has_begun_) {
        begin();
    }

    if (!status_.ok()) {
        LOGGING(base::WARNING) << "Writing to a bad writer fails automatically";
        return status_;
    }

    if (bytes < 0) {
        return Status(base::error::INVALID_ARGUMENT, "Attempted negative write");
    }

    status_ = doWrite(bytes, data);
    if (status_.ok()) {
        size_ += bytes;
    }

    return status_;
}

Status DataWriter::writeData(const std::string& data) {
    return writeData(data.size(), data.data());
}

Status DataWriter::writeData(DataReader* reader, int64 max_bytes) {
    if (!ok()) {
        return status();
    }

    if (!reader->isDone()) {
        if (max_bytes < 0) {
            max_bytes = kINT64max;
        }

        auto reader_remaining = reader->getTotalLength() - reader->getOffset();
        auto remaining = reader_remaining < 0 ? max_bytes : std::min(reader_remaining, max_bytes);
        if (remaining != 0) {
            const int64 default_chunk_size = 1 << 12;
            int64 chunk_size = std::min(remaining, default_chunk_size);
            std::unique_ptr<char[]> buffer(new char[chunk_size]);

            // write data chunks until done or hit an error
            while ((remaining != 0) && !reader->isDone() && ok()) {
                auto read = reader->readIntoBuffer(std::min(remaining, chunk_size), buffer.get());
                writeData(read, buffer.get());
                remaining -= read;
            }
        }
    }

    // if the reader finished with error, propagate it to the writer
    if (reader->error()) {
        setStatus(reader->status());
    }

    return status();
}

DataReader* DataWriter::CreateManagedDataReader(Callback* delete_cb) {
    if (!status().ok()) {
        LOGGING(base::ERROR) << "Error from bad writer";
        return CreateManagedInvalidDataReader(status_, delete_cb);
    }

    return doCreateDataReader(delete_cb);
}

Status DataWriter::doBegin() {
    return Status();
}

Status DataWriter::doEnd() {
    return Status();
}

Status DataWriter::doClear() {
    return Status();
}

}  // namespace serialization
}  // namespace kwc