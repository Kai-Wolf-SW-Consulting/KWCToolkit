// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_SYSTEM_ENVIRONMENT_H_
#define KWCTOOLKIT_SYSTEM_ENVIRONMENT_H_

#include <memory>
#include <string>

namespace kwc {
namespace system {

// Utility class for setting/getting environment variables
class Environment {
  public:
    virtual ~Environment();

    static std::unique_ptr<Environment> create();

    // Gets en environment variable and returns it in |result|.
    // Returns false, if the variable is not set
    virtual bool getEnvVar(const std::string& var_name, std::string* result) = 0;

    // Returns true, if the environment variable is set
    virtual bool hasEnvVar(const std::string& var_name);

    // Sets an environment variable
    // Returns true on success, otherwise returns false
    virtual bool setEnvVar(const std::string& var_name, const std::string& new_value) = 0;

    // Returns true on success, otherwise returns false
    virtual bool unsetEnvVar(const std::string& var_name) = 0;
};

}  // namespace system
}  // namespace kwc

#endif  // KWCTOOLKIT_SYSTEM_ENVIRONMENT_H_
