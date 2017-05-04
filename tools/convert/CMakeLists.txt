cmake_minimum_required(VERSION 3.5)
project(convert)

set(TOOL_CONVERT_GRAMMAR "convert-grammar")
set(TOOL_CONVERT_GRAMMAR ${TOOL_CONVERT_GRAMMAR} PARENT_SCOPE)

set(TOOL_CONVERT_SYNTAX_CONFIG "convert-syntax-config")
set(TOOL_CONVERT_SYNTAX_CONFIG ${TOOL_CONVERT_SYNTAX_CONFIG} PARENT_SCOPE)

# Add convert executables
add_executable(${TOOL_CONVERT_GRAMMAR} Convertible.cpp ConvertGrammar.cpp)
target_link_libraries(${TOOL_CONVERT_GRAMMAR} ${PROJECT_DEV_LIB})

add_executable(${TOOL_CONVERT_SYNTAX_CONFIG} Convertible.cpp ConvertSyntaxConfig.cpp)
target_link_libraries(${TOOL_CONVERT_SYNTAX_CONFIG} ${PROJECT_DEV_LIB})