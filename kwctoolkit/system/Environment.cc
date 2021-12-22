// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/system/Environment.h"

#include <cstdlib>

#include <kwctoolkit/base/Platform.h>
#include <kwctoolkit/strings/StringUtils.h>

#if defined(KWC_OS_WINDOWS)
    #include <windows.h>
#endif

namespace kwc {
namespace system {

namespace {
class EnvironmentImpl : public Environment {
  public:
    bool GetEnvVar(const std::string& var_name, std::string* result) override {
        if (GetEnvVarImpl(var_name, result)) {
            return true;
        }

        // It can happen that some environment variables are uppercase, while other are lowercase.
        // Thus, we're looking to find a variable name with the reverse case, e.g. HTTP_PROXY may
        // be http_proxy
        char first_char = var_name[0];
        std::string alt_case_var;
        if (strings::IsASCIILower(first_char)) {
            alt_case_var = strings::ToUpperASCII(var_name);
        } else if (strings::IsASCIIUpper(first_char)) {
            alt_case_var = strings::ToLowerASCII(var_name);
        } else {
            return false;
        }

        return GetEnvVarImpl(alt_case_var, result);
    }

    bool SetEnvVar(const std::string& var_name, const std::string& new_value) override {
        return SetEnvVarImpl(var_name, new_value);
    }

    bool UnsetEnvVar(const std::string& var_name) override { return UnsetEnvVarImpl(var_name); }

  private:
    bool GetEnvVarImpl(const std::string& var_name, std::string* result) {
#if defined(KWC_OS_MACOS) || defined(KWC_OS_LINUX)
        const char* env_value = getenv(var_name.data());
        if (!env_value)
            return false;
        if (result)
            *result = env_value;
        return true;
#elif defined(KWC_OS_WINDOWS)
        std::unique_ptr<char[]> value(new char[32767]);
        DWORD value_length = ::GetEnvironmentVariableA(var_name.c_str(), value.get(), 32767);
        if (value_length == 0) {
            return false;
        }
        if (result) {
            *result = value.get();
        }
        return true;
#else
    #error not implemented
#endif
    }

    bool SetEnvVarImpl(const std::string& var_name, const std::string& new_value) {
#if defined(KWC_OS_MACOS) || defined(KWC_OS_LINUX)
        // On success, zero is returned
        return !setenv(var_name.data(), new_value.c_str(), 1);
#elif defined(KWC_OS_WINDOWS)
        // On success, a nonzero value is returned
        return !!SetEnvironmentVariableA(var_name.c_str(), new_value.c_str());
#endif
    }

    bool UnsetEnvVarImpl(const std::string& var_name) {
#if defined(KWC_OS_MACOS) || defined(KWC_OS_LINUX)
        // On success, zero is returned
        return !unsetenv(var_name.data());
#elif defined(KWC_OS_WINDOWS)
        // On success, a nonzero value is returned
        return !!SetEnvironmentVariableA(var_name.c_str(), nullptr);
#endif
    }
};
}  // namespace

Environment::~Environment() = default;

// static
std::unique_ptr<Environment> Environment::Create() {
    return std::make_unique<EnvironmentImpl>();
}

bool Environment::HasEnvVar(const std::string& var_name) {
    return GetEnvVar(var_name, nullptr);
}

}  // namespace system
}  // namespace kwc