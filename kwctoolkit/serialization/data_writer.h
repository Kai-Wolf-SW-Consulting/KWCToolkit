// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_SERIALIZATION_DATA_WRITER_H_
#define KWCTOOLKIT_SERIALIZATION_DATA_WRITER_H_

#include <string>

#include "kwctoolkit/base/integral_types.h"
#include "kwctoolkit/base/macros.h"
#include "kwctoolkit/base/status.h"

namespace kwc {
class Callback;
namespace serialization {

class DataReader;
using base::Status;

class DataWriter {
  public:
    virtual ~DataWriter() = default;

    int64 getSize() const { return size_; }

    bool ok() const { return status_.ok(); }

    const Status& status() const { return status_; }

    void clear();

    void begin();

    void end();

    Status writeData(int64 bytes, const char* data);

    Status writeData(const std::string& data);

    Status writeData(DataReader* reader, int64 max_bytes = -1);

    DataReader* createUnmanagedDataReader() { return createManagedDataReader(nullptr); }

    DataReader* createManagedDataReader(Callback* delete_cb);

  protected:
    DataWriter();

    void setStatus(const Status& status) { status_ = status; }

    virtual Status doBegin();

    virtual Status doEnd();

    virtual Status doClear();

    virtual Status doWrite(int64 bytes, const char* data) = 0;

    virtual DataReader* doCreateDataReader(Callback* delete_cb) = 0;

  private:
    int64 size_{0};
    bool has_begun_{false};
    Status status_;

    DISALLOW_COPY_AND_ASSIGN(DataWriter);
};

DataWriter* CreateStringDataWriter(std::string* str);

DataWriter* CreateStringDataWriter();

DataWriter* CreateFileDataWriter(const std::string& path);

}  // namespace serialization
}  // namespace kwc

#endif  // KWCTOOLKIT_SERIALIZATION_DATA_WRITER_H_
