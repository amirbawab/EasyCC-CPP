#!/bin/bash

# Generate header file
make generate_files

# NOTE: The following 2 steps might not be required in other project.
# However, for this program it is needed because the example is
# a subfolder of EasyCC-CPP
cp -ar ../../bin/include/ EasyCC-CPP/bin/
./run_cmake.sh

# Build project
make calculator
