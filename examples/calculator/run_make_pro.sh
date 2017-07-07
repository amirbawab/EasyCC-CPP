#!/bin/bash

# Generate header file
make generate_files

# NOTE: The following step might not be required in other project.
# However, for this program it is needed because the example is
# a subfolder of EasyCC-CPP
cp -ar ../../bin/include/ EasyCC-CPP/bin/

# Refresh c++ header files to include the new generated ones
./run_cmake.sh

# Build project
make calculator
