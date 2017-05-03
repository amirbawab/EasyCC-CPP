cmake_minimum_required(VERSION 3.5)
project(easycc)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Set targets
set(PROJECT_DEV_LIB easyccdev)
set(PROJECT_PRO_LIB easyccpro)

# Configure directory of output file
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Store src cpp files in a variable
file(GLOB_RECURSE PROJECT_SOURCE_FILES "src/*.cpp")
list(REMOVE_ITEM PROJECT_SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/src/easycc/EasyCCDev.cpp"
        "${PROJECT_SOURCE_DIR}/src/easycc/EasyCCPro.cpp")

# Include easycc header files (e.g. same as -I flag)
include_directories(include)
include_directories(rapidjson/include)
include_directories(tools/include)

# Check if generate files exists, if so use them
if(EXISTS "${PROJECT_SOURCE_DIR}/bin/generated/cpp")
    file(GLOB_RECURSE GENERATED_FILES ${PROJECT_SOURCE_DIR}/bin/generated/cpp/*.cpp)
else()
    file(GLOB_RECURSE GENERATED_FILES ${PROJECT_SOURCE_DIR}/tools/convert/templates/*.cpp)
endif()
set(PROJECT_SOURCE_FILES ${PROJECT_SOURCE_FILES} ${GENERATED_FILES})

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

# Add subdirectory CMakeLists.txt
add_subdirectory(tools/convert)
add_subdirectory(googletest)
add_subdirectory(test)
add_subdirectory(examples/calculator-dev)
add_subdirectory(examples/calculator-pro)

# Explicitely generate header files
add_custom_target(
        generate_files
        COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_SOURCE_DIR}/bin/generated/cpp
        COMMAND ${TOOL_CONVERT_GRAMMAR}
        -t ${PROJECT_SOURCE_DIR}/tools/convert/templates/StaticGrammar.cpp
        -o ${PROJECT_SOURCE_DIR}/bin/generated/cpp/StaticGrammar.cpp
        -i ${PROJECT_SOURCE_DIR}/resources/src/calculator/syntax_grammar.txt
        DEPENDS ${TOOL_CONVERT_GRAMMAR}
)