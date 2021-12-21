// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "serialization/DataReader.h"

#include <algorithm>
#include <cstdlib>
#include <memory>

#include "base/Callback.h"
#include "base/Check.h"

namespace kwc {
namespace serialization {
namespace {
const int64 kDefaultBufferSize = 1 << 13;  // 8KB

struct FreeDeleter {
    inline void operator()(void* ptr) const { free(ptr); }
};
}  // namespace

DataReader::DataReader(Callback* delete_cb)
    : delete_cb_(delete_cb), done_(false), total_length_(-1), offset_(0) {}

DataReader::~DataReader() {
    if (delete_cb_ != nullptr) {
        delete_cb_->run();
    }
}

bool DataReader::isSeekable() const {
    return false;
}

int64 DataReader::setOffset(int64 position) {
    if (position < 0) {
        setStatus(base::Status(base::error::INVALID_ARGUMENT,
                               "Negative offset: " + std::to_string(position)));
        offset_ = -1;
        return -1;
    }

    if (position < offset_ || offset_ < 0) {
        done_ = false;
    }
    if (!status_.ok()) {
        status_ = base::Status();
    }

    offset_ = doSetOffset(position);
    if (offset_ < 0 && status_.ok()) {
        setStatus(
            base::Status(base::error::UNKNOWN, "Could not seek to: " + std::to_string(position)));
    }
    return offset_;
}

int64 DataReader::readIntoBuffer(int64 max_bytes, char* storage) {
    if (max_bytes < 0) {
        setStatus(base::Status(base::error::INVALID_ARGUMENT, "negative read"));
    }

    int64 total_read = 0;
    while (total_read < max_bytes && !isDone()) {
        auto read = doReadIntoBuffer(max_bytes - total_read, storage + total_read);
        KWC_CHECK_LE(0, read);
        if (read < 0) {
            setStatus(base::Status(base::error::UNKNOWN, "Internal error"));
            return 0;
        }
        offset_ += read;
        total_read += read;
    }
    return total_read;
}

int64 DataReader::readIntoString(int64 max_bytes, std::string* into) {
    if (max_bytes < 0) {
        setStatus(base::Status(base::error::INVALID_ARGUMENT, "Invalid argument"));
        return 0;
    }
    auto len = getTotalLength();
    if (len >= 0) {
        auto remaining = len - getOffset();
        if (remaining > 0) {
            into->reserve(remaining + into->size());
        }
    }

    std::unique_ptr<char, FreeDeleter> buffer(reinterpret_cast<char*>(malloc(kDefaultBufferSize)));
    char* storage = buffer.get();
    if (storage == nullptr) {
        setStatus(base::Status(base::error::UNKNOWN, "Out of memory"));
        return 0;
    }

    int64 total_read = 0;
    while (total_read < max_bytes && !isDone()) {
        const int64 bytes_to_read = std::min(kDefaultBufferSize, max_bytes - total_read);
        auto read = doReadIntoBuffer(bytes_to_read, storage);
        KWC_CHECK_LE(0, read);
        if (read < 0) {
            setStatus(base::Status(base::error::UNKNOWN, "Internal error"));
            return 0;
        }
        if (read != 0) {
            offset_ += read;
            total_read += read;
            into->append(storage, read);
        }
    }
    return total_read;
}

bool DataReader::readUntil(const std::string& pattern, std::string* consumed) {
    consumed->clear();
    auto result = doAppendUntil(pattern, consumed);
    offset_ += consumed->size();
    return result;
}

int64 DataReader::doSetOffset(int64 /*position*/) {
    setStatus(base::Status(base::error::NOT_FOUND, "Reader cannot seek to offset"));
    return -1;
}

bool DataReader::doAppendUntil(const std::string& pattern, std::string* consumed) {
    const char* find = pattern.c_str();
    auto found_offset = consumed->size();
    while (*find != 0) {
        if (isDone()) {
            return false;
        }
        auto original_size = consumed->size();
        char c;
        if (doReadIntoBuffer(1, &c) == 0) {
            continue;
        }
        consumed->push_back(c);
        if (c == *find) {
            ++find;
            continue;
        }

        // We are not matching the pattern, so we reset it.
        // Buf if the pattern may has a repeated substring then we might still
        // be in the middle of matching it from an offset between the old
        // |found_offset| and the end of the |consumed| string. Therefore we're
        // looking for onw.
        if (found_offset == original_size) {
            ++found_offset;
            continue;
        }

        const char* end_consumed = consumed->c_str() + consumed->size();
        while ((found_offset = consumed->find(pattern.c_str(), found_offset + 1, 1)) !=
               std::string::npos) {
            const char* have;
            for (have = consumed->c_str() + found_offset + 1, find = pattern.c_str() + 1;
                 have < end_consumed && *find == *have; ++find, ++have) {}
            if (have == end_consumed) {
                break;
            }

            // Start over again from a |found_offset| closer to the end of the
            // consumed string
        }

        if (found_offset == std::string::npos) {
            // The consumed string so far doesn't end with a substring of the
            // pattern
            found_offset = consumed->size();
            find = pattern.c_str();
        }
    }

    return true;
}

void DataReader::setStatus(const base::Status& status) {
    status_ = status;
    if (!status.ok()) {
        done_ = true;
    }
}

void DataReader::setTotalLength(int64 length) {
    total_length_ = length;
    if (length == 0) {
        done_ = true;
    }
}

class InvalidDataReader : public DataReader {
  public:
    InvalidDataReader(base::Status status, Callback* delete_cb)
        : DataReader(delete_cb), status_(status) {
        setStatus(status);
    }

  protected:
    int64 doSetOffset(int64 /*offset*/) override {
        setStatus(status_);
        return -1;
    }

    int64 doReadIntoBuffer(int64 /*max_bytes*/, char* /*storage*/) override { return 0; }

  private:
    base::Status status_;
};

DataReader* CreateManagedInvalidDataReader(const base::Status& status, Callback* delete_cb) {
    return new InvalidDataReader(status, delete_cb);
}

}  // namespace serialization
}  // namespace kwc
