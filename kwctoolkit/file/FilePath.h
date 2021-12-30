// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_FILE_FILE_PATH_H_
#define KWCTOOLKIT_FILE_FILE_PATH_H_

#include <string>

namespace kwc {
namespace file {

// Utility for filepath name representation
// We use this until std::filesystem (C++17) becomes available. FilePath objects may be used
// anywhere where paths to objects inside the filesystem are used. In addition several methods for
// common file operations are provided, which should be favoured over direct string manipulations.
class FilePath {
  public:
    using StringType = std::string;
    using CharType = std::string::value_type;

    FilePath() = default;
    FilePath(const FilePath& other);
    explicit FilePath(StringType path);

    FilePath& operator=(const FilePath& other);
    bool operator==(const FilePath& other) const;
    bool operator!=(const FilePath& other) const;
    bool operator<(const FilePath& other) const;
    const StringType& value() const;

    static bool isSeperator(CharType ch);
    FilePath append(const std::string& component) const;
    FilePath append(const FilePath& component) const;
    FilePath baseName() const;
    FilePath directoryName() const;
    FilePath stripTrailingSeparators() const;

  private:
    void stripTrailingSeparatorsInternal();
    std::string path_;
};

}  // namespace file
}  // namespace kwc

#endif  // KWCTOOLKIT_FILE_FILE_PATH_H_
