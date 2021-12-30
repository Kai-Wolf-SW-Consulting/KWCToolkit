// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_SYSTEM_FEATURE_LIST_H_
#define KWCTOOLKIT_SYSTEM_FEATURE_LIST_H_

#include <functional>
#include <map>
#include <memory>
#include <string>

#include "kwctoolkit/base/Macros.h"

namespace kwc {
namespace base {

// Specifies whether a given feature is enabled or disabled by default
// Please note that the actual runtime state_ may be different, due to a command line switch*/
enum FeatureState { FEATURE_DISABLED_BY_DEFAULT, FEATURE_ENABLED_BY_DEFAULT };

// This struct is used to define the default state_ for a given feature
struct Feature {
    // Name of the feature that can be enabled/disabled on the command line as well
    const char* const name;

    // Default state_ for this feature
    const FeatureState default_state;
};

// Enable a given feature during runtime (via command line switch) for applications
//
// The basic use case for this class is to enable/disable any feature that can be toggled and that
// is defined through a Feature struct, e.g.:
//
//     const base::Feature kMyCoolFeature {
//         "MyCoolFeature", base::FEATURE_ENABLED_BY_DEFAULT
//     }
//
// Then, anywhere in the code_ base, one can check for this feature using:
//
//     if (base::FeatureList::IsEnabled(kMyCoolFeature)) {
//         // New, cool feature goes here...
//     }
//
// The above call to the FeatureList will take into account any command-line flags to
// enable/disable the given feature as well as its default state_, to determine whether the feature
// is on.
//
// Features can be explicitly forced to be on/off by specifying a list of comma-separated feature
// names via the following command-line flags:
//
//     --enable_features=MyCoolFeature,MyOtherCoolFeature
//     --disable_features=MyBrokenFeature,MyUnwantedFeature,MyOtherExperimentalFeature
//
// Please note that this class should be used as a singleton, but doesn't adhere to the standard way
// of implementing Singletons using a private/static constructor in order to have full control over
// its initialization sequence. Hence, the intended use is to create an instance of this class and
// fully initialize it, before calling SetInstance()
class FeatureList {
  public:
    FeatureList() = default;
    ~FeatureList() = default;

    enum OverrideState {
        OVERRIDE_USE_DEFAULT,
        OVERRIDE_DISABLE_FEATURE,
        OVERRIDE_ENABLE_FEATURE,
    };

    void initializeFromCommandLine(const std::string& enabled_features,
                                   const std::string& disabled_features);

    static bool initializeInstance(const std::string& enabled_features,
                                   const std::string& disabled_features);

    static bool isEnabled(const Feature& feature);

    static FeatureList* getInstance();

    static void setInstance(std::unique_ptr<FeatureList> instance);

  private:
    DISALLOW_COPY_AND_ASSIGN(FeatureList);
    bool isFeatureEnabled(const Feature& feature);
    void finalizeInitialization();

    std::map<std::string, OverrideState, std::less<>> overrides_;
    bool initialized_ = false;
};

}  // namespace base

}  // namespace kwc

#endif  // KWCTOOLKIT_BASE_FEATURE_LIST_H_
