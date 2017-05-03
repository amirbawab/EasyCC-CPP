cmake_minimum_required(VERSION 3.5)
project(easycctests)
set(PROJECT_TEST "easycctests")

# Copy resources
set(TEST_RES_PATH "../resources/test")
file(COPY "${TEST_RES_PATH}/grammar/" DESTINATION "${CMAKE_BINARY_DIR}/bin/${TEST_RES_PATH}/grammar/")
file(COPY "${TEST_RES_PATH}/graph/" DESTINATION "${CMAKE_BINARY_DIR}/bin/${TEST_RES_PATH}/graph/")
file(COPY "${TEST_RES_PATH}/firstfollow/" DESTINATION "${CMAKE_BINARY_DIR}/bin/${TEST_RES_PATH}/firstfollow/")

enable_testing()

# Add gtest header files (e.g. same as -I flag)
include_directories(../googletest/include)

# Store easycc test files to a variable
file(GLOB_RECURSE TEST_SRC *.cpp)

# Add test executable
add_executable(${PROJECT_TEST} ${PROJECT_SOURCE_FILES} ${TEST_SRC})

# Link libraries to the test executable only
target_link_libraries(${PROJECT_TEST} gtest gtest_main)
add_test(${PROJECT_TEST} ${PROJECT_TEST})
