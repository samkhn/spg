#!/bin/bash

find . -name '*.c' -o -name '*.cc' -o -name '*.cpp' -o -name '*.cxx' -o -name '*.h' -o -name '*.hpp' -o -name '*.hxx' -o -name '*.tpp' | xargs clang-format -i
