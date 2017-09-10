# <img src="logo.png" alt="EasyCC-C++ logo" width="200"> [![Build Status](https://travis-ci.org/amirbawab/EasyCC-CPP.svg?branch=master)](https://travis-ci.org/amirbawab/EasyCC-CPP)
**Version**: 1.1.0

## Github
Link: [Release 1.1.0](https://github.com/amirbawab/EasyCC-CPP/releases/tag/v1.1.0)

## About
EasyCC C++ (Easy Compiler Compiler written in C++) is a library allowing users to easily develop their own programming language. The project does not require wrting any line of code for the lexical and syntax analysis phases. The configurations of the latters are provided as JSON files to the library. Adding the logic for a programming language is done by simply registering semantic action handlers in order to gradually build the structure of the input and eventually generating output code.

## Usage
*Note: The library depends on Boost 1.63.0, make sure it is installed before compiling the project (Instructions can be found in `.travis.yml` file).*
### CMakeLists.txt
```
cmake_minimum_required(VERSION 3.5)
project(myProject)
set(MYPROJECT_DEV_EXEC "myproject_dev")
set(MYPROJECT_PRO_EXEC "myproject_pro")

add_subdirectory(EasyCC-CPP)
add_compile_options(-std=c++11)

# Configure directory of output file
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include easycc header files
include_directories(EasyCC-CPP/include)

# Add the executables
add_executable(${MYPROJECT_DEV_EXEC} maindev.cpp)
add_executable(${MYPROJECT_PRO_EXEC} mainpro.cpp)

# Link library to the executable
target_link_libraries(${MYPROJECT_DEV_EXEC} easyccdev)
target_link_libraries(${MYPROJECT_PRO_EXEC} easyccpro)
```
For a full `CMakeLists.txt` example, check [BashClass](https://github.com/amirbawab/BashClass) which uses EasyCC C++ library

## Difference between `easyccdev` and `easyccpro` libraries
Both libraries use the same Lexical and Syntax algorithms, and switching from one library to another requires minor changes.

* `easyccdev` (EasyCC development mode) takes the JSON files as arguments in the final executable. This is useful during the development phase because the program does not need to be recompiled to apply changes in the JSON files.
* `easyccpro` (EasyCC production mode) takes the JSON files as arguments to the `cmake` program. The compile process for your program will be composed of two steps. First is embedding the JSON files into the executable, and  second is compiling your program with the generated files.

## Contribution
* Feel free to contribute

