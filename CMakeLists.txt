cmake_minimum_required(VERSION 3.5)
project(easycc)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Set targets
set(PROJECT_TEST "easycctests")
set(PROJECT_DEV_LIB easyccdev)
set(PROJECT_PRO_LIB easyccpro)
set(PROJECT_EXEC_1 "calculator")
set(TOOL_FIRST_FOLLOW "firstfollow")

# Copy resources
file(GLOB resources_TEST "resources/test/*")
file(COPY ${resources_TEST} DESTINATION "${CMAKE_BINARY_DIR}/bin/resources/test/")

# Configure directory of output file
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include easycc header files (e.g. same as -I flag)
include_directories(include)

# Store src cpp files in a variable
file(GLOB_RECURSE PROJECT_SOURCE_FILES "src/*/*.cpp")

# Include rapidjson header files
include_directories(rapidjson/include)

# Add boost
add_definitions(-DBOOST_LOG_DYN_LINK)
find_package(Boost 1.63.0 COMPONENTS log log_setup thread date_time filesystem system REQUIRED)
link_libraries(${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${Boost_LIBRARIES})

# Add -std=c++11 flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Link libraries to all targets
link_libraries(-lpthread)
link_libraries(${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})

# [TARGET 1]
# Add library target
add_library(${PROJECT_DEV_LIB} ${PROJECT_SOURCE_FILES})
# [/TARGET 1]

# Add gtest CMakeLists.txt to build it automatically
add_subdirectory(googletest)
enable_testing()

# Add gtest header files (e.g. same as -I flag)
include_directories(googletest/include)

# Store easycc test files to a variable
file(GLOB_RECURSE easycctests_SRC "test/*.h" "test/*.cpp")

# [TARGET 2]
# Add test executable
add_executable(${PROJECT_TEST} ${PROJECT_SOURCE_FILES} ${easycctests_SRC})

# Link libraries to the test executable only
target_link_libraries(${PROJECT_TEST} gtest gtest_main)
add_test(${PROJECT_TEST} ${PROJECT_TEST})
# [/TARGET 2]

# [TARGET 3]
# Add first and follow set executable
# add_executable(${TOOL_FIRST_FOLLOW} ${PROJECT_SOURCE_FILES} tools/first_follow/ConvertFF.h tools/first_follow/ConvertFF.cpp)
# [/TARGET 3]

# [TARGET 4]
# Add example executable
add_executable(${PROJECT_EXEC_1} ${PROJECT_SOURCE_FILES} examples/calculator/calculator.cpp)
# [/TARGET 4]
