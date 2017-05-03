cmake_minimum_required(VERSION 3.5)
project(calculator-pro)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(EXAMPLE_CALCULATOR_PRO "calculator-pro")

# Add example executable
add_executable(${EXAMPLE_CALCULATOR_PRO} CalculatorPro.cpp)
target_link_libraries(${EXAMPLE_CALCULATOR_PRO} ${PROJECT_PRO_LIB})
