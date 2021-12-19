// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE file in the root directory. For the
// list of contributors see the AUTHORS file in the same directory.

#ifndef KWCTOOLKIT_UTILS_REGEX_H_
#define KWCTOOLKIT_UTILS_REGEX_H_

#include <regex>
#include <string>

namespace kwc {
namespace utils {

enum class RegexStatus {
    STANDARD,
    ANYTHING_BUT,
    MAYBE,
    START_OF_LINE,
    SOMETHING_BUT,
    END_OF_LINE,
};

class Regex {
  public:
    Regex& add(const std::string& value);
    Regex& startOfLine(const std::string& value);
    Regex& withoutCaseSensitivity();
    Regex& then(const std::string& value);
    Regex& maybe(const std::string& value);
    Regex& alternative(const std::string& value);
    Regex& anythingBut(const std::string& value);
    Regex& anything();
    Regex& something();
    Regex& somethingBut(const std::string& value);
    Regex& find(const std::string& value);
    Regex& endOfLine(const std::string& value);

    bool match(const std::string& value);
    bool search(const std::string& value);

    operator Regex() const { return *this; }

  private:
    const int deviation_maybe_ = 2;
    const int deviation_start_of_line_ = 1;
    const int deviation_end_of_line_ = 2;
    const int deviation_anything_but_s_first_ = 1;
    const int deviation_anything_but_ = 3;
    const int deviation_something_but_s_first_ = 1;
    const int deviation_something_but_ = 3;

    RegexStatus reg_stat_ = RegexStatus::STANDARD;
    RegexStatus last_stat_ = RegexStatus::STANDARD;
    bool no_case_sensitivity_ = false;
    std::string reg_{""};
};
}  // namespace utils
}  // namespace kwc

#endif  // KWCTOOLKIT_UTILS_REGEX_H_
