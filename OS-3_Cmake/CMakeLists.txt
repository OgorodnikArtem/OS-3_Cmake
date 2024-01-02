cmake_minimum_required(VERSION 3.10)
project(MyThreadsProject)

set(CMAKE_CXX_STANDARD 98)

add_executable(MyThreadsProject OS-3_Cmake.cpp)

target_link_libraries(MyThreadsProject PRIVATE kernel32)
