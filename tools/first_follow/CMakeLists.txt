cmake_minimum_required(VERSION 3.5)
project(firstfollow)
set(TOOL_FIRST_FOLLOW "firstfollow")

# Add first and follow set executable
add_executable(${TOOL_FIRST_FOLLOW} ConvertFF.cpp)
target_link_libraries(${TOOL_FIRST_FOLLOW} ${PROJECT_DEV_LIB})
