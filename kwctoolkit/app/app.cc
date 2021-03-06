// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/app/app.h"

#include <string>

#include "kwctoolkit/app/app_info.h"
#include "kwctoolkit/strings/string_utils.h"

namespace kwc {
namespace app {

std::string App::getProjectName() const {
    return APPLICATION_NAME;
}

std::string App::getProjectDescription() const {
    return APPLICATION_DESCRIPTION;
}

std::string App::getProjectCodeName() const {
    std::string project_code_name;
    const char* p = &APPLICATION_CODENAME[0];
    while (*p != '\0') {
        project_code_name += strings::ToLowerASCII(*p++);
    }
    return project_code_name;
}

std::string App::getProjectVendorId() const {
    return APPLICATION_VENDOR_ID;
}

std::string App::getProjectVendorName() const {
    return APPLICATION_VENDOR_NAME;
}

std::string App::getProjectVendorUrl() const {
    return APPLICATION_VENDOR_URL;
}

std::string App::getProjectId() const {
    return APPLICATION_ID;
}

int App::getProjectMajorVersion() const {
    return APPLICATION_VERSION_MAJOR;
}

int App::getProjectMinorVersion() const {
    return APPLICATION_VERSION_MINOR;
}

int App::getProjectPatchVersion() const {
    return APPLICATION_VERSION_PATCH;
}

std::string App::getProjectVersion() const {
    return APPLICATION_VERSION;
}

std::string App::getProjectCopyright() const {
    return APPLICATION_COPYRIGHT;
}

}  // namespace app
}  // namespace kwc
