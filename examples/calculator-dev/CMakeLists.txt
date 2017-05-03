cmake_minimum_required(VERSION 3.5)
project(calculator-dev)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(EXAMPLE_CALCULATOR_DEV "calculator-dev")

# Add example executable
add_executable(${EXAMPLE_CALCULATOR_DEV} CalculatorDev.cpp)
target_link_libraries(${EXAMPLE_CALCULATOR_DEV} ${PROJECT_DEV_LIB})
