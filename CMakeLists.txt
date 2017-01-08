cmake_minimum_required(VERSION 3.5)
project(EasyCC_CPP)
set(CMAKE_VERBOSE_MAKEFILE ON)

##### RESOURCES #####
file(GLOB resources_SRC "resources/src/*")
file(GLOB resources_TEST "resources/test/*")
file(COPY ${resources_SRC} DESTINATION "${CMAKE_BINARY_DIR}/bin/resources/src/")
file(COPY ${resources_TEST} DESTINATION "${CMAKE_BINARY_DIR}/bin/resources/test/")

#### OUTPUT ####
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

##### TEST ######
ADD_SUBDIRECTORY (gtest-1.6.0)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

file(GLOB_RECURSE easycctests_SRC
    "test/*.h"
    "test/*.cpp"
)

add_executable(easycctests ${easycctests_SRC})
target_link_libraries(easycctests gtest gtest_main)
add_test(easycctests easycctests)

##### APP #####
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    lexical/graph/Graph.cpp
    lexical/graph/Graph.h
    lexical/graph/State.cpp
    lexical/graph/State.h
    lexical/Lexical.cpp
    lexical/Lexical.h
    rapidjson/error/en.h
    rapidjson/error/error.h
    rapidjson/internal/biginteger.h
    rapidjson/internal/diyfp.h
    rapidjson/internal/dtoa.h
    rapidjson/internal/ieee754.h
    rapidjson/internal/itoa.h
    rapidjson/internal/meta.h
    rapidjson/internal/pow10.h
    rapidjson/internal/regex.h
    rapidjson/internal/stack.h
    rapidjson/internal/strfunc.h
    rapidjson/internal/strtod.h
    rapidjson/internal/swap.h
    rapidjson/msinttypes/inttypes.h
    rapidjson/msinttypes/stdint.h
    rapidjson/allocators.h
    rapidjson/document.h
    rapidjson/encodedstream.h
    rapidjson/encodings.h
    rapidjson/filereadstream.h
    rapidjson/filewritestream.h
    rapidjson/fwd.h
    rapidjson/istreamwrapper.h
    rapidjson/memorybuffer.h
    rapidjson/memorystream.h
    rapidjson/ostreamwrapper.h
    rapidjson/pointer.h
    rapidjson/prettywriter.h
    rapidjson/rapidjson.h
    rapidjson/reader.h
    rapidjson/schema.h
    rapidjson/stream.h
    rapidjson/stringbuffer.h
    rapidjson/writer.h
        src/EasyCC.cpp lexical/config/Config.cpp lexical/config/Config.h lexical/token/LexicalToken.cpp lexical/token/LexicalToken.h lexical/config/Messages.cpp lexical/config/Messages.h syntax/grammar/Grammar.cpp syntax/grammar/Grammar.h syntax/Syntax.cpp syntax/Syntax.h)

add_executable(EasyCC_CPP ${SOURCE_FILES})

