cmake_minimum_required(VERSION 3.5)
project(calculator)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CALCULATOR_DEV_EXEC "calculatordev")
set(CALCULATOR_PRO_EXEC "calculator")

# Add easycc CMakeLists.txt to build it automatically
add_subdirectory(${PROJECT_SOURCE_DIR}/../../ ${PROJECT_BINARY_DIR}/EasyCC-CPP)

# Compile with -std=c++11 flag
add_compile_options(-std=c++11)

# Configure directory of output file
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include calculator header files (e.g. same as -I flag)
include_directories(include)
include_directories(../../include)
include_directories(../../thirdparty/rapidjson/include)

# Store cpp files in a variable
file(GLOB_RECURSE CALC_PROJECT_SOURCE_FILES src/*/*.cpp)
list(REMOVE_ITEM CALC_PROJECT_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/src/calculator/maindev.cpp"
        "${PROJECT_SOURCE_DIR}/src/calculator/mainpro.cpp")

# Add the executables
add_executable(${CALCULATOR_DEV_EXEC} ${CALC_PROJECT_SOURCE_FILES} src/calculator/maindev.cpp)
add_executable(${CALCULATOR_PRO_EXEC} ${CALC_PROJECT_SOURCE_FILES} src/calculator/mainpro.cpp)

# Link library to the executable
target_link_libraries(${CALCULATOR_DEV_EXEC} easyccdev)
target_link_libraries(${CALCULATOR_PRO_EXEC} easyccpro)
