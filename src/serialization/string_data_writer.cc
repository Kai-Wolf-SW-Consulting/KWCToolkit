// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include <string>

#include "base/integral_types.h"
#include "base/status.h"
#include "serialization/data_reader.h"
#include "serialization/data_writer.h"

namespace kwc {
class Callback;

namespace serialization {
class StringDataWriter : public DataWriter {
  public:
    StringDataWriter() : storage_(&local_storage_) {}
    explicit StringDataWriter(std::string* storage) : storage_(storage) {}
    ~StringDataWriter() override = default;

    Status doBegin() override { return doClear(); }

    Status doWrite(int64 bytes, const char* buffer) override {
        storage_->append(buffer, bytes);
        return Status();
    }

    Status doClear() override {
        storage_->clear();
        return Status();
    }

    DataReader* doCreateDataReader(Callback* delete_cb) override {
        return CreateManagedInMemoryDataReader(*storage_, delete_cb);
    }

  private:
    std::string local_storage_;
    std::string* storage_;
};

DataWriter* CreateStringDataWriter(std::string* str) {
    return new StringDataWriter(str);
}

DataWriter* CreateStringDataWriter() {
    return new StringDataWriter();
}

}  // namespace serialization
}  // namespace kwc