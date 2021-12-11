// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include <algorithm>
#include <cstring>
#include <string>
#include <utility>

#include "base/callback.h"
#include "base/integral_types.h"
#include "serialization/data_reader.h"

namespace kwc {
namespace serialization {

class InMemoryDataReader : public DataReader {
  public:
    InMemoryDataReader(std::string data, Callback* delete_cb)
        : DataReader(delete_cb), data_(std::move(data)) {
        setTotalLength(data_.size());
    }

    ~InMemoryDataReader() override = default;

    bool isSeekable() const override { return true; }

  protected:
    int64 doReadIntoBuffer(int64 max_bytes, char* storage) override {
        auto remaining = static_cast<int64>(data_.size() - getOffset());
        if (remaining == 0) {
            setDone(true);
            return 0;
        }

        auto read = std::min(max_bytes, remaining);
        if (read > 0) {
            memcpy(storage, data_.data() + getOffset(), read);
            if (read == remaining) {
                setDone(true);
            }
        }
        return read;
    }

    int64 doSetOffset(int64 position) override {
        if (position > data_.size()) {
            return data_.size();
        }

        return position;
    }

    bool doAppendUntil(const std::string& pattern, std::string* consumed) override {
        auto start = getOffset();
        auto found = data_.find(pattern, start);
        auto end = found == std::string::npos ? data_.size() : found + pattern.size();
        consumed->append(data_.substr(start, end - start));
        return found != std::string::npos;
    }

  private:
    std::string data_;
};

DataReader* CreateUnmanagedInMemoryDataReader(const std::string& data) {
    return new InMemoryDataReader(data, nullptr);
}

DataReader* CreateManagedInMemoryDataReader(const std::string& data, Callback* delete_cb) {
    return new InMemoryDataReader(data, delete_cb);
}

DataReader* CreateManagedInMemoryDataReader(std::string* data) {
    return CreateManagedInMemoryDataReader(std::string(*data), DeletePointerCallback(data));
}

}  // namespace serialization
}  // namespace kwc