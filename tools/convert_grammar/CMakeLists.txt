cmake_minimum_required(VERSION 3.5)
project(firstfollow)
set(TOOL_CONVERT_GRAMMAR "firstfollow")
set(TOOL_CONVERT_GRAMMAR ${TOOL_CONVERT_GRAMMAR} PARENT_SCOPE)

# Add first and follow set executable
add_executable(${TOOL_CONVERT_GRAMMAR} ConvertGrammar.cpp)
target_link_libraries(${TOOL_CONVERT_GRAMMAR} ${PROJECT_DEV_LIB})
