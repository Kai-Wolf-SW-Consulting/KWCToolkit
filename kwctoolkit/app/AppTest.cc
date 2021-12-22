// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/app/App.h"

#include <gtest/gtest.h>

#include "kwctoolkit/app/AppInfo.h"
#include "kwctoolkit/strings/StringUtils.h"

using namespace kwc;

class AppTest : public ::testing::Test {
  protected:
    app::App app_info;
};

TEST_F(AppTest, GetProjectName) {
    ASSERT_STREQ(app_info.getProjectName().c_str(), APPLICATION_NAME);
}

TEST_F(AppTest, GetProjectCodeName) {
    for (const auto ch : app_info.getProjectCodeName()) {
        ASSERT_TRUE(strings::IsASCIILower(ch));
    }
}

// getProjectCodeName
TEST_F(AppTest, GetProjectVersion) {
    ASSERT_EQ(app_info.getProjectMajorVersion(), APPLICATION_VERSION_MAJOR);
    ASSERT_EQ(app_info.getProjectMinorVersion(), APPLICATION_VERSION_MINOR);
    ASSERT_EQ(app_info.getProjectPatchVersion(), APPLICATION_VERSION_PATCH);
    ASSERT_STREQ(app_info.getProjectVersion().c_str(), APPLICATION_VERSION);
}

TEST_F(AppTest, GetProjectDetails) {
    ASSERT_STREQ(app_info.getProjectVendorId().c_str(), APPLICATION_VENDOR_ID);
    ASSERT_STREQ(app_info.getProjectVendorName().c_str(), APPLICATION_VENDOR_NAME);
    ASSERT_STREQ(app_info.getProjectId().c_str(), APPLICATION_ID);
    ASSERT_STREQ(app_info.getProjectCopyright().c_str(), APPLICATION_COPYRIGHT);
}