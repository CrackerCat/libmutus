cmake_minimum_required(VERSION 3.17)
project(test)

set(CMAKE_CXX_STANDARD 20)

include_directories(..)

add_executable(test main.cpp)