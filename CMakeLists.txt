cmake_minimum_required(VERSION 3.5)
project(easycc)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Set targets
set(PROJECT_TEST "easycctests")
set(PROJECT_DEV_LIB easyccdev)
set(PROJECT_PRO_LIB easyccpro)
set(EXAMPLE_CALCULATOR "calculator")
set(TOOL_FIRST_FOLLOW "firstfollow")

# Copy resources
set(TEST_RES_PATH "resources/test")
file(COPY "${TEST_RES_PATH}/grammar/" DESTINATION "${CMAKE_BINARY_DIR}/bin/${TEST_RES_PATH}/grammar/")
file(COPY "${TEST_RES_PATH}/graph/" DESTINATION "${CMAKE_BINARY_DIR}/bin/${TEST_RES_PATH}/graph/")
file(COPY "${TEST_RES_PATH}/firstfollow/" DESTINATION "${CMAKE_BINARY_DIR}/bin/${TEST_RES_PATH}/firstfollow/")

# Configure directory of output file
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include easycc header files (e.g. same as -I flag)
include_directories(include)

# Store src cpp files in a variable
file(GLOB_RECURSE PROJECT_SOURCE_FILES "src/*/*.cpp")
list(REMOVE_ITEM PROJECT_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/src/easycc/EasyCCDev.cpp"
        "${PROJECT_SOURCE_DIR}/src/easycc/EasyCCPro.cpp")

# Include rapidjson header files
include_directories(rapidjson/include)

# Include tools header files
include_directories(tools/include)

# Check if generate files exists, if so use them
if(EXISTS "${PROJECT_SOURCE_DIR}/bin/include/easycc/generated")
    include_directories(bin/include)
else()
    include_directories(tools/cond_include)
endif()

# Add boost
add_definitions(-DBOOST_LOG_DYN_LINK)
find_package(Boost 1.63.0 COMPONENTS log log_setup thread date_time filesystem system REQUIRED)
link_libraries(${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${Boost_LIBRARIES})

# Add -std=c++11 flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Link libraries to all targets
link_libraries(-lpthread)
link_libraries(${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})

# Add development library target
add_library(${PROJECT_DEV_LIB} ${PROJECT_SOURCE_FILES} src/easycc/EasyCCDev.cpp)

# Add production library target
add_library(${PROJECT_PRO_LIB} ${PROJECT_SOURCE_FILES} src/easycc/EasyCCPro.cpp)

# Add gtest CMakeLists.txt to build it automatically
add_subdirectory(googletest)
enable_testing()

# Add gtest header files (e.g. same as -I flag)
include_directories(googletest/include)

# Store easycc test files to a variable
file(GLOB_RECURSE easycctests_SRC "test/*.h" "test/*.cpp")

# Add test executable
add_executable(${PROJECT_TEST} ${PROJECT_SOURCE_FILES} ${easycctests_SRC})

# Link libraries to the test executable only
target_link_libraries(${PROJECT_TEST} gtest gtest_main)
add_test(${PROJECT_TEST} ${PROJECT_TEST})

# Add first and follow set executable
add_executable(${TOOL_FIRST_FOLLOW} tools/first_follow/ConvertFF.cpp tools/include/easycc/tools/ConvertFF.h)
target_link_libraries(${TOOL_FIRST_FOLLOW} ${PROJECT_DEV_LIB})

# Add example executable
add_executable(${EXAMPLE_CALCULATOR} examples/calculator/calculator.cpp)
target_link_libraries(${EXAMPLE_CALCULATOR} ${PROJECT_DEV_LIB})

# Generate grammar header file
add_custom_command(
        TARGET ${PROJECT_PRO_LIB} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/bin/include/easycc/generated
        COMMAND ${TOOL_FIRST_FOLLOW}
        ARGS -t ${PROJECT_SOURCE_DIR}/tools/include/easycc/generated/GrammarFF.h
        -o ${PROJECT_SOURCE_DIR}/bin/include/easycc/generated/GrammarFF.h
        ${PROJECT_SOURCE_DIR}/resources/src/calculator/syntax_grammar.txt
        DEPENDS ${TOOL_FIRST_FOLLOW}
        VERBATIM)
