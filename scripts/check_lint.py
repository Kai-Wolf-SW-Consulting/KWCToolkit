#!/usr/bin/env python3
#
# Copyright (c) 2021, Kai Wolf - SW Consulting. All rights reserved.
# For the licensing terms see LICENSE file in the root directory. For the
# list of contributors see the AUTHORS file in the same directory.

"""Ensures that all source files follow consistent style rules

This script runs over the code base and ensures various style related (linting) rules such
as consistent inclusion guards, copyright notice etc.
"""

from os.path import abspath, basename, dirname, join, relpath, sep
from os import walk
from re import compile as reg_compile, sub
from sys import exit

spectre_dir = abspath(dirname("__file__"))

# Inclusion guards should start with 'KWCTOOLKIT_', maybe continue with 'SRC' or 'INC'
# and then match the actual path to the filename
matcher_guard = "KWCTOOLKIT_(?:SRC_|INCLUDE_)?)([a-zA-z0-9]+)"
header_guard_regex = reg_compile(r"(#ifndef %s\n(#define %s" % (matcher_guard, matcher_guard))
copyright_regex = reg_compile(r"Copyright \([cC]\) \d{4}(-\d{4})?, Kai Wolf - SW Consulting")


def check_copyright(current_file):
    """Ensures that current source file includes copyright"""
    with open(current_file, "r", encoding='utf-8') as src_file:
        for line in src_file.readlines():
            if copyright_regex.search(line):
                break
        else:
            print("Error: No copyright message found in", current_file)
            exit(-1)


def check_header_guard(current_file):
    """Ensures standard conforming include guards"""
    if not current_file.endswith(".h"):
        return

    suffix = relpath(current_file, spectre_dir)
    parts = sub(r"\.", "/", sub(r"(src|include)/", "", suffix)).split('/')
    guard = '_'.join(map(str.upper, [sub(r'(?<!^)(?=[A-Z])', '_', w) for w in parts])) + '_'
    repl = "\\1%s\n\\3%s" % (guard, guard)
    with open(current_file, 'r+', encoding='utf-8') as src_file:
        original = src_file.read()
        modified = ""
        if header_guard_regex.search(original):
            modified = sub(header_guard_regex, repl, original, 1)
        if modified:
            src_file.seek(0)
            src_file.truncate(0)
            src_file.write(modified)
            src_file.flush()
            src_file.close()


def main():
    valid_endings = ["cc", "h"]
    exclude = []
    for target_dir in ["src", "include"]:
        for current_path, dirs, files in walk(join(spectre_dir, target_dir)):
            dirs[:] = [d for d in dirs if d not in exclude]
            for afile in files:
                if not basename(afile).split(".")[-1] in valid_endings: continue
                current_file = join(current_path, afile)
                check_header_guard(current_file)
                check_copyright(current_file)

    return 0


if __name__ == "__main__":
    exit(main())

