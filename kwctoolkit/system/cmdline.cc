// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "kwctoolkit/system/cmdline.h"

#include <utility>

#include "kwctoolkit/file/file_path.h"
#include "kwctoolkit/strings/string_utils.h"

using kwc::file::FilePath;

namespace kwc {
namespace base {

const CommandLine::CharType kSwitchTerminator[] = "--";
const CommandLine::CharType kSwitchValueSeparator[] = "=";
const CommandLine::CharType* kSwitchPrefixes[] = {"--", "-"};  // NOLINT
CommandLine* CommandLine::current_process = nullptr;

CommandLine::CommandLine(NoProgram) : argv_(1), start_args_(1) {}

CommandLine::CommandLine(const FilePath& program) : argv_(1), start_args_(1) {
    setProgram(program);
}

CommandLine::CommandLine(int argc, const CommandLine::CharType* const* argv)
    : argv_(1), start_args_(1) {
    initFromArgv(argc, argv);
}

CommandLine::CommandLine(const CommandLine::StringVector& argv) : argv_(1), start_args_(1) {
    initFromArgv(argv);
}

bool CommandLine::init(int argc, const char* const* argv) {
    if (current_process) {
        return false;
    }

    // TODO: remove new call
    current_process = new CommandLine(NoProgram::NO_PROGRAM);
    current_process->initFromArgv(argc, argv);
    return true;
}

void CommandLine::reset() {
    delete current_process;
    current_process = nullptr;
}

void CommandLine::initFromArgv(int argc, const CharType* const* argv) {
    StringVector new_argv;
    for (int i = 0; i < argc; ++i) {
        new_argv.push_back(argv[i]);
    }
    initFromArgv(new_argv);
}

void CommandLine::initFromArgv(const CommandLine::StringVector& argv) {
    argv_ = StringVector(1);
    start_args_ = 1;
    switches_.clear();
    setProgram(argv.empty() ? file::FilePath() : file::FilePath(argv[0]));
    auto parse_switches = true;
    for (std::size_t i = 1; i < argv.size(); ++i) {
        CommandLine::StringType arg = argv[i];
        arg = strings::TrimString(arg, " ", strings::TRIM_ALL);
        CommandLine::StringType switch_string;
        CommandLine::StringType switch_value;
        parse_switches &= (arg != kSwitchTerminator);
        if (parse_switches && isSwitch(arg, &switch_string, &switch_value)) {
            appendSwitch(switch_string, switch_value);
        } else {
            appendArg(arg);
        }
    }
}

CommandLine::StringType CommandLine::getCommandLineString() const {
    return getCommandLineStringInternal();
}

CommandLine::StringType CommandLine::getArgumentsString() const {
    return getArgumentsStringInternal();
}

const CommandLine::StringVector& CommandLine::argv() const {
    return argv_;
}

file::FilePath CommandLine::getProgram() const {
    return file::FilePath(argv_[0]);
}

void CommandLine::setProgram(const file::FilePath& program) {
    argv_[0] = strings::TrimString(program.value(), " ", strings::TRIM_ALL);
}

bool CommandLine::hasSwitch(const CommandLine::StringType& switch_string) const {
    // TODO(kwc): tolower first before checking
    return switches_.find(switch_string) != switches_.end();
}

bool CommandLine::hasSwitch(const CharType switch_constant[]) const {
    return hasSwitch(StringType(switch_constant));
}

CommandLine::StringType CommandLine::getSwitchValue(
    const CommandLine::StringType& switch_string) const {
    return getSwitchValueNative(switch_string);
}

file::FilePath CommandLine::getSwitchValuePath(const CommandLine::StringType& switch_string) const {
    return file::FilePath(getSwitchValueNative(switch_string));
}

CommandLine::StringType CommandLine::getSwitchValueNative(
    const CommandLine::StringType& switch_string) const {
    auto result = switches_.find(switch_string);
    return result == switches_.end() ? CommandLine::StringType() : result->second;
}

const CommandLine::SwitchMap& CommandLine::getSwitches() const {
    return switches_;
}

void CommandLine::appendSwitch(const CommandLine::StringType& switch_string) {
    appendSwitch(switch_string, StringType());
}

void CommandLine::appendSwitchPath(const CommandLine::StringType& switch_string,
                                   const file::FilePath& path) {
    appendSwitch(switch_string, path.value());
}

void CommandLine::appendSwitch(const CommandLine::StringType& switch_string,
                               const CommandLine::StringType& value) {
    auto prefix_length = getSwitchPrefixLength(switch_string);
    auto insert = switches_.insert(std::make_pair(switch_string.substr(prefix_length), value));
    if (!insert.second) {
        insert.first->second = value;
    }
    StringType combined_switch_str(switch_string);
    if (prefix_length == 0) {
        combined_switch_str = kSwitchPrefixes[0] + combined_switch_str;
    }
    if (!value.empty()) {
        combined_switch_str += kSwitchValueSeparator + value;
    }
    argv_.insert(argv_.begin() + start_args_++, combined_switch_str);
}

CommandLine::StringVector CommandLine::getArgs() const {
    StringVector args(argv_.begin() + start_args_, argv_.end());
    auto switch_terminator = std::find(args.begin(), args.end(), kSwitchTerminator);
    if (switch_terminator != args.end()) {
        args.erase(switch_terminator);
    }
    return args;
}

void CommandLine::appendArg(const CommandLine::StringType& value) {
    argv_.push_back(value);
}

void CommandLine::appendArgPath(const file::FilePath& path) {
    argv_.push_back(path.value());
}

CommandLine::StringType CommandLine::getCommandLineStringInternal() const {
    StringType string(argv_[0]);
    StringType params(getArgumentsStringInternal());
    if (!params.empty()) {
        string.append(StringType(" "));
        string.append(params);
    }
    return string;
}

CommandLine::StringType CommandLine::getArgumentsStringInternal() const {
    StringType params;
    bool parse_switches = true;
    for (std::size_t i = 1; i < argv_.size(); ++i) {
        StringType arg = argv_[i];
        StringType switch_string;
        StringType switch_value;
        parse_switches &= arg != kSwitchTerminator;
        if (i > 1) {
            params.append(StringType(" "));
        }
        if (parse_switches && isSwitch(arg, &switch_string, &switch_value)) {
            params.append(switch_string);
            if (!switch_value.empty()) {
                params.append(kSwitchValueSeparator + switch_value);
            }
        } else {
            params.append(arg);
        }
    }
    return params;
}

bool CommandLine::isSwitch(const CommandLine::StringType& string,
                           CommandLine::StringType* switch_string,
                           CommandLine::StringType* switch_value) const {
    switch_string->clear();
    switch_value->clear();
    auto prefix_length = getSwitchPrefixLength(string);
    if (prefix_length == 0 || prefix_length == string.length()) {
        return false;
    }
    auto equals_pos = string.find(kSwitchValueSeparator);
    *switch_string = string.substr(0, equals_pos);
    if (equals_pos != CommandLine::StringType::npos) {
        *switch_value = string.substr(equals_pos + 1);
    }
    return true;
}

std::size_t CommandLine::getSwitchPrefixLength(const CommandLine::StringType& string) const {
    for (auto idx : kSwitchPrefixes) {
        CommandLine::StringType prefix(idx);
        if (string.compare(0, prefix.length(), prefix) == 0) {
            return prefix.length();
        }
    }
    return 0;
}

}  // namespace base
}  // namespace kwc