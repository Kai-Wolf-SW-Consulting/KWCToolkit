// Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
// For the licensing terms see LICENSE n the root directory. For the
// list of contributors see the AUTHORS n the same directory.

#include "system/Cmdline.h"

#include <gtest/gtest.h>

#include "base/ArraySize.h"
#include "file/FilePath.h"

using namespace kwc::base;
using kwc::file::FilePath;

TEST(CommandLineTest, Constructor) {
    const CommandLine::CharType* argv[] = {"program",
                                           "--foo=",
                                           "-bAr",
                                           "-spaetzel=pierogi",
                                           "-baz",
                                           "flim",
                                           "--other-switches=--dog=canine --cat=feline",
                                           "-spaetzle=Crepe",
                                           "-=loosevalue",
                                           "-",
                                           "FLAN",
                                           "a",
                                           "--input-translation=45--output-rotation",
                                           "--",
                                           "--",
                                           "--not-a-switch",
                                           "\"in the time of submarines...\"",
                                           "unquoted arg-with-space"};
    CommandLine cl(arraysize(argv), argv);

    EXPECT_FALSE(cl.getCommandLineString().empty());
    EXPECT_FALSE(cl.hasSwitch("cruller"));
    EXPECT_FALSE(cl.hasSwitch("flim"));
    EXPECT_FALSE(cl.hasSwitch("program"));
    EXPECT_FALSE(cl.hasSwitch("dog"));
    EXPECT_FALSE(cl.hasSwitch("cat"));
    EXPECT_FALSE(cl.hasSwitch("output-rotation"));
    EXPECT_FALSE(cl.hasSwitch("not-a-switch"));
    EXPECT_FALSE(cl.hasSwitch("--"));

    EXPECT_EQ(FilePath("program").value(), cl.getProgram().value());
    EXPECT_TRUE(cl.hasSwitch("foo"));
    EXPECT_FALSE(cl.hasSwitch("bar"));
    EXPECT_TRUE(cl.hasSwitch("baz"));
    EXPECT_TRUE(cl.hasSwitch("spaetzle"));
    EXPECT_TRUE(cl.hasSwitch("other-switches"));
    EXPECT_TRUE(cl.hasSwitch("input-translation"));

    EXPECT_EQ("Crepe", cl.getSwitchValue("spaetzle"));
    EXPECT_EQ("", cl.getSwitchValue("foo"));
    EXPECT_EQ("", cl.getSwitchValue("bar"));
    EXPECT_EQ("", cl.getSwitchValue("cruller"));
    EXPECT_EQ("--dog=canine --cat=feline", cl.getSwitchValue("other-switches"));
    EXPECT_EQ("45--output-rotation", cl.getSwitchValue("input-translation"));

    const auto args = cl.getArgs();
    ASSERT_EQ(8U, args.size());

    auto iter = args.begin();
    EXPECT_EQ("flim", *iter);
    ++iter;
    EXPECT_EQ("-", *iter);
    ++iter;
    EXPECT_EQ("FLAN", *iter);
    ++iter;
    EXPECT_EQ("a", *iter);
    ++iter;
    EXPECT_EQ("--", *iter);
    ++iter;
    EXPECT_EQ("--not-a-switch", *iter);
    ++iter;
    EXPECT_EQ("\"in the time of submarines...\"", *iter);
    ++iter;
    EXPECT_EQ("unquoted arg-with-space", *iter);
    ++iter;
    EXPECT_TRUE(iter == args.end());
}

TEST(CommandLineTest, EmptyString) {
    CommandLine cl_from_argv(0, nullptr);
    EXPECT_TRUE(cl_from_argv.getCommandLineString().empty());
    EXPECT_TRUE(cl_from_argv.getProgram().value().empty());
    EXPECT_EQ(1U, cl_from_argv.argv().size());
    EXPECT_TRUE(cl_from_argv.getArgs().empty());
}

TEST(CommandLineTest, AppendSwitches) {
    std::string switch1 = "switch1";
    std::string switch2 = "switch2";
    std::string value2 = "value";
    std::string switch3 = "switch3";
    std::string value3 = "a value with spaces";
    std::string switch4 = "switch4";
    std::string value4 = "\"a value with quotes\"";

    CommandLine cl(FilePath("Program"));

    cl.appendSwitch(switch1);
    cl.appendSwitch(switch2, value2);
    cl.appendSwitch(switch3, value3);
    cl.appendSwitch(switch4, value4);

    EXPECT_TRUE(cl.hasSwitch(switch1));
    EXPECT_TRUE(cl.hasSwitch(switch2));
    EXPECT_EQ(value2, cl.getSwitchValue(switch2));
    EXPECT_TRUE(cl.hasSwitch(switch3));
    EXPECT_EQ(value3, cl.getSwitchValue(switch3));
    EXPECT_TRUE(cl.hasSwitch(switch4));
    EXPECT_EQ(value4, cl.getSwitchValue(switch4));
}

TEST(CommandLineTest, AppendSwitchesWithDoubleDashes) {
    const CommandLine::CharType* raw_argv[] = {"prog", "--", "--arg1"};
    CommandLine cl(arraysize(raw_argv), raw_argv);

    cl.appendSwitch("switch1");
    // std::cout << cl.getCommandLineString() << std::endl;
    cl.appendSwitch("switch2", "foo");
    // std::cout << cl.getCommandLineString() << std::endl;
    cl.appendArg("--arg2");
    // std::cout << cl.getCommandLineString() << std::endl;

    EXPECT_EQ("prog --switch1 --switch2=foo -- --arg1 --arg2", cl.getCommandLineString());
    auto cl_argv = cl.argv();
    ASSERT_EQ(cl_argv.size(), 6);
    EXPECT_EQ("prog", cl_argv[0]);
    EXPECT_EQ("--switch1", cl_argv[1]);
    EXPECT_EQ("--switch2=foo", cl_argv[2]);
    EXPECT_EQ("--", cl_argv[3]);
    EXPECT_EQ("--arg1", cl_argv[4]);
    EXPECT_EQ("--arg2", cl_argv[5]);
}
