// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "system/feature_list.h"

#include <utility>
#include <vector>

#include "base/check.h"
#include "strings/string_split.h"

namespace kwc {
namespace base {
namespace {
// Global instance of FeatureList that was set via FeatureList::setInstance()
FeatureList* g_feature_list_instance = nullptr;

std::vector<std::string> SplitFeatureListString(const std::string& input) {
    using namespace strings;
    return SplitString(input, ",", WHITESPACE_TRIM, SPLIT_WANT_NONEMPTY);
}
}  // namespace

void FeatureList::initializeFromCommandLine(const std::string& enabled_features,
                                            const std::string& disabled_features) {
    CHECK(!initialized_);
    for (const auto& feature_name : SplitFeatureListString(disabled_features)) {
        overrides_.insert(std::make_pair(feature_name, OVERRIDE_DISABLE_FEATURE));
    }

    for (const auto& feature_name : SplitFeatureListString(enabled_features)) {
        overrides_.insert(std::make_pair(feature_name, OVERRIDE_ENABLE_FEATURE));
    }
}

bool FeatureList::initializeInstance(const std::string& enabled_features,
                                     const std::string& disabled_features) {
    bool instance_existed_before = false;
    if (g_feature_list_instance) {
        delete g_feature_list_instance;
        g_feature_list_instance = nullptr;
        instance_existed_before = true;
    }

    std::unique_ptr<FeatureList> feature_list(new FeatureList);
    feature_list->initializeFromCommandLine(enabled_features, disabled_features);
    FeatureList::setInstance(std::move(feature_list));
    return !instance_existed_before;
}

bool FeatureList::isEnabled(const Feature& feature) {
    if (!g_feature_list_instance) {
        return feature.default_state == FEATURE_ENABLED_BY_DEFAULT;
    }
    return g_feature_list_instance->isFeatureEnabled(feature);
}

FeatureList* FeatureList::getInstance() {
    return g_feature_list_instance;
}

void FeatureList::setInstance(std::unique_ptr<FeatureList> instance) {
    CHECK(!g_feature_list_instance);
    instance->finalizeInitialization();
    g_feature_list_instance = instance.release();
}

bool FeatureList::isFeatureEnabled(const Feature& feature) {
    CHECK(initialized_);
    auto it = overrides_.find(feature.name);
    if (it != overrides_.end()) {
        const auto& state = it->second;
        if (state != OVERRIDE_USE_DEFAULT) {
            return state == OVERRIDE_ENABLE_FEATURE;
        }
    }
    return feature.default_state == FEATURE_ENABLED_BY_DEFAULT;
}

void FeatureList::finalizeInitialization() {
    CHECK(!g_feature_list_instance);
    initialized_ = true;
}

}  // namespace base
}  // namespace kwc