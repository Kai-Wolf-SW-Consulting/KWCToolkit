// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#include "utils/Regex.h"

#include "base/Assert.h"
#include "strings/StringUtils.h"

namespace kwc {
namespace utils {

Regex& Regex::add(const std::string& value) {
    last_stat_ = RegexStatus::STANDARD;
    reg_ += "(" + value + ")";
    return *this;
}

Regex& Regex::startOfLine(const std::string& value) {
    reg_ += "^((" + value + "))";
    reg_stat_ = RegexStatus::START_OF_LINE;
    return *this;
}

Regex& Regex::withoutCaseSensitivity() {
    no_case_sensitivity_ = true;
    return *this;
}

Regex& Regex::then(const std::string& value) {
    return add(value);
}

Regex& Regex::maybe(const std::string& value) {
    last_stat_ = RegexStatus::STANDARD;
    reg_ += "((" + value + "))?";
    reg_stat_ = RegexStatus::MAYBE;
    return *this;
}

Regex& Regex::alternative(const std::string& value) {
    if (last_stat_ != RegexStatus::STANDARD) {
        reg_stat_ = last_stat_;
    }

    switch (reg_stat_) {
        case RegexStatus::MAYBE:
            last_stat_ = RegexStatus::MAYBE;
            reg_.insert(reg_.length() - deviation_maybe_, "|(" + value + ")");
            break;
        case RegexStatus::START_OF_LINE:
            last_stat_ = RegexStatus::START_OF_LINE;
            reg_.insert(reg_.length() - deviation_start_of_line_, "|(" + value + ")");
            break;
        case RegexStatus::END_OF_LINE:
            last_stat_ = RegexStatus::END_OF_LINE;
            reg_.insert(reg_.length() - deviation_end_of_line_, "|(" + value + ")");
            break;
        case RegexStatus::ANYTHING_BUT:
            last_stat_ = RegexStatus::ANYTHING_BUT;
            if (strings::IsWhitespace(value.front())) {
                reg_ += "|\\S*";
            } else if (reg_.at(reg_.length() - deviation_anything_but_s_first_) == 'S') {
                reg_ += "|(?!(.*" + value + ")).*";
            } else {
                reg_.insert(reg_.length() - deviation_anything_but_, "|(.*" + value + ")");
            }
            break;
        case RegexStatus::SOMETHING_BUT:
            last_stat_ = RegexStatus::SOMETHING_BUT;
            if (strings::IsWhitespace(value.front())) {
                reg_ += "|\\S+";
            } else if (reg_.at(reg_.length() - deviation_something_but_s_first_) == 'S') {
                reg_ += "|(?!(.*" + value + ")).+";
            } else {
                reg_.insert(reg_.length() - deviation_something_but_, "|(.+" + value + ")");
            }
            break;
        case RegexStatus::STANDARD:
            last_stat_ = RegexStatus::STANDARD;
            reg_ += "|(" + value + ")";
            break;
        default: UNREACHABLE(); break;
    }

    reg_stat_ = RegexStatus::STANDARD;
    return *this;
}

Regex& Regex::anythingBut(const std::string& value) {
    last_stat_ = RegexStatus::STANDARD;

    if (strings::IsWhitespace(value.front())) {
        reg_ += "\\S*";
    } else {
        reg_ += "(?!(.*" + value + ")).*";
    }

    reg_stat_ = RegexStatus::ANYTHING_BUT;
    return *this;
}

Regex& Regex::anything() {
    last_stat_ = RegexStatus::STANDARD;
    reg_ += ".*";
    return *this;
}

Regex& Regex::something() {
    last_stat_ = RegexStatus::STANDARD;
    reg_ += ".+";
    return *this;
}

Regex& Regex::somethingBut(const std::string& value) {
    last_stat_ = RegexStatus::STANDARD;

    if (strings::IsWhitespace(value.front())) {
        reg_ += "\\S*";
    } else {
        reg_ += "(?!(.*" + value + ")).+";
    }

    reg_stat_ = RegexStatus::SOMETHING_BUT;
    return *this;
}

Regex& Regex::find(const std::string& value) {
    return add(value);
}

Regex& Regex::endOfLine(const std::string& value) {
    last_stat_ = RegexStatus::STANDARD;
    reg_ += "((" + value + "))$";
    return *this;
}

bool Regex::match(const std::string& value) {
    std::regex r;
    if (no_case_sensitivity_) {
        r = std::regex(reg_, std::regex_constants::icase);
    } else {
        r = std::regex(reg_);
    }

    return std::regex_match(value, r);
}

bool Regex::search(const std::string& value) {
    std::regex r;
    if (no_case_sensitivity_) {
        r = std::regex(reg_, std::regex_constants::icase);
    } else {
        r = std::regex(reg_);
    }

    std::smatch sm;

    return std::regex_search(value, sm, r);
}

}  // namespace utils
}  // namespace kwc
