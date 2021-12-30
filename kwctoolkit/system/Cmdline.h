// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_SYSTEM_CMDLINE_H_
#define KWCTOOLKIT_SYSTEM_CMDLINE_H_

#include <cstddef>
#include <map>
#include <string>
#include <vector>

#include "kwctoolkit/base/Macros.h"

namespace kwc {
namespace file {
class FilePath;
}
namespace base {

enum class NoProgram { NO_PROGRAM };

class CommandLine {
  public:
    using StringType = std::string;
    using CharType = std::string::value_type;
    using StringVector = std::vector<StringType>;
    using SwitchMap = std::map<std::string, StringType>;

    // A program name must be explicitly specified
    DISALLOW_DEFAULT_CTOR(CommandLine);

    // create only switches and arguments
    explicit CommandLine(NoProgram no_program);

    // create command line options from |program| as argv[0]
    explicit CommandLine(const file::FilePath& program);

    // create command line options from argument list
    CommandLine(int argc, const CharType* const* argv);
    explicit CommandLine(const StringVector& argv);

    static bool init(int argc, const char* const* argv);
    static void reset();
    void initFromArgv(int argc, const CharType* const* argv);
    void initFromArgv(const StringVector& argv);
    StringType getCommandLineString() const;
    StringType getArgumentsString() const;
    const StringVector& argv() const;
    file::FilePath getProgram() const;
    void setProgram(const file::FilePath& program);
    bool hasSwitch(const StringType& switch_string) const;
    bool hasSwitch(const CharType switch_constant[]) const;
    StringType getSwitchValue(const StringType& switch_string) const;
    file::FilePath getSwitchValuePath(const StringType& switch_string) const;
    StringType getSwitchValueNative(const StringType& switch_string) const;
    const SwitchMap& getSwitches() const;

    void appendSwitch(const StringType& switch_string);
    void appendSwitchPath(const StringType& switch_string, const file::FilePath& path);
    void appendSwitch(const StringType& switch_string, const StringType& value);
    StringVector getArgs() const;

    void appendArg(const StringType& value);
    void appendArgPath(const file::FilePath& path);

  private:
    StringType getCommandLineStringInternal() const;
    StringType getArgumentsStringInternal() const;
    bool isSwitch(const StringType& string,
                  StringType* switch_string,
                  StringType* switch_value) const;
    std::size_t getSwitchPrefixLength(const StringType& string) const;

    StringVector argv_;
    SwitchMap switches_;
    std::size_t start_args_;
    static CommandLine* current_process;
};

}  // namespace base
}  // namespace kwc

#endif  // KWCTOOLKIT_BASE_CMDLINE_H_
