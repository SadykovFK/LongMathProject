cmake_minimum_required(VERSION 3.20)

project(LongNum)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++20")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(LongMathBits)
add_subdirectory(LongMathBits)

add_executable(pi pi.cpp)
target_link_libraries(pi LongMathBits)

add_executable(test test.cpp)
target_link_libraries(test LongMathBits)