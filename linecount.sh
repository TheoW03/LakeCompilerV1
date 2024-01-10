#!/bin/bash

find . -name '*.cpp' | grep -v '/build/CMakeFiles/3.22.1/CompilerIdCXX/CMakeCXXCompilerId.cpp' | xargs wc -l
