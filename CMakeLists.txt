cmake_minimum_required(VERSION 3.20)

project(LongNum)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++20")

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(LongMathVer1)

add_executable(MAIN pi.cpp LongMathVer1/LongMathBits.cpp)