cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME "bpfsec-project")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set compiler
set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(${PROJECT_NAME})

add_subdirectory(bpfkern)
add_subdirectory(bpfsec)
