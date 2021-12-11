// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_SERIALIZATION_DATA_READER_H_
#define KWCTOOLKIT_SERIALIZATION_DATA_READER_H_

#include <istream>
#include <string>

#include "base/integral_types.h"
#include "base/macros.h"
#include "base/status.h"

namespace kwc {
class Callback;
namespace serialization {

class DataReader {
  public:
    virtual ~DataReader();

    virtual bool isSeekable() const;

    bool isDone() const { return done_; }

    bool error() const { return !status_.ok(); }

    bool ok() const { return status_.ok(); }

    base::Status status() const { return status_; }

    int64 getOffset() const { return offset_; }

    int64 setOffset(int64 position);

    bool reset() { return setOffset(0) == 0; }

    int64 readIntoBuffer(int64 max_bytes, char* storage);

    int64 readIntoString(int64 max_bytes, std::string* into);

    std::string readRemainingToString() {
        std::string result;
        readIntoString(kINT64max, &result);
        return result;
    }

    int64 getTotalLength() const { return total_length_; }

    bool readUntil(const std::string& pattern, std::string* consumed);

  protected:
    explicit DataReader(Callback* delete_cb);

    virtual int64 doReadIntoBuffer(int64 max_bytes, char* storage) = 0;
    virtual int64 doSetOffset(int64 position);

    virtual bool doAppendUntil(const std::string& pattern, std::string* consumed);

    void setStatus(const base::Status& status);

    void setDone(bool done) { done_ = done; }

    void setTotalLength(int64 length);

  private:
    Callback* delete_cb_;
    bool done_;
    int64 total_length_;
    int64 offset_;
    base::Status status_;

    DISALLOW_COPY_AND_ASSIGN(DataReader);
};

DataReader* CreateManagedInvalidDataReader(const base::Status& status, Callback* delete_cb);

inline DataReader* CreateUnmanagedInvalidDataReader(const base::Status& status) {
    return CreateManagedInvalidDataReader(status, nullptr);
}

DataReader* CreateManagedFileDataReader(const std::string& path, Callback* delete_cb);

DataReader* CreateUnmanagedFileDataReader(const std::string& path);

DataReader* CreateManagedInMemoryDataReader(const std::string& data, Callback* delete_cb);

DataReader* CreateUnmanagedInMemoryDataReader(const std::string& data);

DataReader* CreateManagedInMemoryDataReader(std::string* data);

inline DataReader* CreateManagedInMemoryDataReader(const std::string& data) {
    return CreateManagedInMemoryDataReader(new std::string(data));
}

DataReader* CreateManagedIstreamDataReader(std::istream* stream, Callback* delete_cb);

DataReader* CreateManagedIstreamDataReader(std::istream* stream, int64 length, Callback* delete_cb);

DataReader* CreateUnmanagedIstreamDataReader(std::istream* stream);

DataReader* CreateUnmanagedIstreamDataReader(std::istream* stream, int64 length);

}  // namespace serialization
}  // namespace kwc

#endif  // KWCTOOLKIT_SERIALIZATION_DATA_READER_H_
