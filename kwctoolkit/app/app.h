// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_APP_APP_H_
#define KWCTOOLKIT_APP_APP_H_

#include <string>

#include "kwctoolkit/base/compiler.h"

namespace kwc {
namespace app {

// This class provides some utility functions to read application details such as project name,
// version number etc. It basically reads all the specified application details from the project's
// CMakeLists.txt, which itself is autogenerated by CMake and put into app_info.h
class App {
  public:
    // Returns the name of the project. May contain spaces, upper/lower letters and numbers
    KWC_NO_DISCARD std::string getProjectName() const;

    // Returns a short description text for this project. May contain spaces, upper/lower letters
    // and numbers
    KWC_NO_DISCARD std::string getProjectDescription() const;

    // Returns the code_ name of the project. Should be all lowercase without any spaces in name
    KWC_NO_DISCARD std::string getProjectCodeName() const;

    // Returns the vendor id of the project. Contains dot connected lowercase words, usually the
    // reverse company url such as com.example
    KWC_NO_DISCARD std::string getProjectVendorId() const;

    // Returns the vendor name of the project. May contain spaces upper/lower letters or numbers.
    // Usually the company name is taken here
    KWC_NO_DISCARD std::string getProjectVendorName() const;

    // Return the vendor URL of the project
    KWC_NO_DISCARD std::string getProjectVendorUrl() const;

    // Returns the project id. Contains dot connected lowercase words, usually the company url such
    // as example.com
    KWC_NO_DISCARD std::string getProjectId() const;

    // Returns the project's major version. This value is usually incremented for every major
    // release
    KWC_NO_DISCARD int getProjectMajorVersion() const;

    // Returns the project's minor version. This value is usually incremented for every minor
    // release
    KWC_NO_DISCARD int getProjectMinorVersion() const;

    // Returns the project's patch version. This value is usually incremented for every released
    // patch version
    KWC_NO_DISCARD int getProjectPatchVersion() const;

    // Returns the project version. Contains the project major, minor and patch version as well as
    // the application version type such as SNAPSHOT for example
    KWC_NO_DISCARD std::string getProjectVersion() const;

    // Returns the project copyright terms
    KWC_NO_DISCARD std::string getProjectCopyright() const;
};

}  // namespace app
}  // namespace kwc

#endif  // KWCTOOLKIT_APP_APP_H_
