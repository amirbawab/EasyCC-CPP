cmake_minimum_required(VERSION 3.5)
project(easycctests)
set(PROJECT_TEST "easycctests")

enable_testing()

# Add gtest header files (e.g. same as -I flag)
include_directories(../thirdparty/googletest/include)

# Store easycc test files to a variable
file(GLOB_RECURSE TEST_SRC *.cpp)

# Add test executable
add_executable(${PROJECT_TEST} ${PROJECT_SOURCE_FILES} ${TEST_SRC})

# Link libraries to the test executable only
target_link_libraries(${PROJECT_TEST} gtest gtest_main)
add_test(${PROJECT_TEST} ${PROJECT_TEST})
