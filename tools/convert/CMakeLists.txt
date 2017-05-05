cmake_minimum_required(VERSION 3.5)
project(convert)

set(TOOL_CONVERT_RESOURCES "convert-resources")
set(TOOL_CONVERT_RESOURCES ${TOOL_CONVERT_RESOURCES} PARENT_SCOPE)

# Add convert executables
add_executable(${TOOL_CONVERT_RESOURCES} ConvertResources.cpp)
target_link_libraries(${TOOL_CONVERT_GRAMMAR} ${PROJECT_DEV_LIB})
