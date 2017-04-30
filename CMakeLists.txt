cmake_minimum_required(VERSION 3.5)
project(easycc)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(PROJECT_TEST "easycctests")
set(PROJECT_LIB easyccls)

# Copy resources
file(GLOB resources_SRC "resources/src/*")
file(GLOB resources_TEST "resources/test/*")
file(COPY ${resources_SRC} DESTINATION "${CMAKE_BINARY_DIR}/bin/resources/src/")
file(COPY ${resources_TEST} DESTINATION "${CMAKE_BINARY_DIR}/bin/resources/test/")

# Configure directory of output file
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Include rapidjson header files (e.g. same as -I flag)
include_directories(rapidjson/include)

# Add boost
add_definitions(-DBOOST_LOG_DYN_LINK)
find_package(Boost 1.63.0 COMPONENTS log log_setup thread date_time filesystem system REQUIRED)
link_libraries(${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES} ${Boost_LIBRARIES})

# Add -std=c++11 flag
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# Store source files in a variables
set(SOURCE_FILES
        lexical/graph/Graph.cpp
        lexical/graph/Graph.h
        lexical/graph/State.cpp
        lexical/graph/State.h
        lexical/Lexical.cpp
        lexical/Lexical.h
        lexical/config/LexicalConfig.cpp
        lexical/config/LexicalConfig.h
        lexical/token/LexicalToken.cpp
        lexical/token/LexicalToken.h
        lexical/config/LexicalMessages.cpp
        lexical/config/LexicalMessages.h
        syntax/grammar/Grammar.cpp
        syntax/grammar/Grammar.h
        syntax/Syntax.cpp
        syntax/Syntax.h
        syntax/config/SyntaxMessages.cpp
        syntax/config/SyntaxMessages.h
        syntax/config/SyntaxConfig.cpp
        syntax/config/SyntaxConfig.h
        easycc/EasyCC.cpp
        easycc/EasyCC.h)

# Add the primary executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES} main.cpp)

# Add library
add_library(${PROJECT_LIB} ${SOURCE_FILES})

# Link libraries to the executbale
target_link_libraries(${PROJECT_NAME} -lpthread)
target_link_libraries(${PROJECT_NAME} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY})

# Add gtest CMakeLists.txt to build it automatically
add_subdirectory(googletest)
enable_testing()

# Add gtest header files (e.g. same as -I flag)
include_directories(${gtest_SOURCE_DIR}/include)

# Add easycc test files to a variable
file(GLOB_RECURSE easycctests_SRC "test/*.h" "test/*.cpp")

# Add test executable
add_executable(${PROJECT_TEST} ${SOURCE_FILES} ${easycctests_SRC})

# Link libraries to the executable
target_link_libraries(${PROJECT_TEST} gtest gtest_main)
add_test(${PROJECT_TEST} ${PROJECT_TEST})
