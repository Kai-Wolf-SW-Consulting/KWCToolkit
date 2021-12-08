// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "system/environment.h"

#include <cstdlib>

#include "base/platform.h"
#include "strings/string_utils.h"

#if defined(OS_WINDOWS)
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
#if defined(OS_MACOS) || defined(OS_LINUX)
        const char* env_value = getenv(var_name.data());
        if (!env_value)
            return false;
        if (result)
            *result = env_value;
        return true;
#elif defined(OS_WINDOWS)
        DWORD value_length =
            ::GetEnvironmentVariable(strings::UTF8ToWide(var_name).c_str(), nullptr, 0);
        if (value_length == 0)
            return false;
        if (result) {
            std::unique_ptr<wchar_t[]> value(new wchar_t[value_length]);
            ::GetEnvironmentVariable(strings::UTF8ToWide(var_name).c_str(), value.get(),
                                     value_length);
            *result = strings::WideToUTF8(value.get());
        }
        return true;
#else
    #error not implemented
#endif
    }

    bool SetEnvVarImpl(const std::string& var_name, const std::string& new_value) {
#if defined(OS_MACOS) || defined(OS_LINUX)
        // On success, zero is returned
        return !setenv(var_name.data(), new_value.c_str(), 1);
#elif defined(OS_WINDOWS)
        // On success, a nonzero value is returned
        return !!SetEnvironmentVariable(strings::UTF8ToWide(var_name).c_str(),
                                        strings::UTF8ToWide(new_value).c_str());
#endif
    }

    bool UnsetEnvVarImpl(const std::string& var_name) {
#if defined(OS_MACOS) || defined(OS_LINUX)
        // On success, zero is returned
        return !unsetenv(var_name.data());
#elif defined(OS_WINDOWS)
        // On success, a nonzero value is returned
        return !!SetEnvironmentVariable(strings::UTF8ToWide(var_name).c_str(), nullptr);
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