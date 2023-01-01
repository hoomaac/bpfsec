cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME "bpfsec-project")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT DEFINED ENV{VCPKG_ROOT})
    message(FATAL_ERROR "vcpkg root is not defined")
endif()

set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

# set compiler
set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(${PROJECT_NAME})

add_subdirectory(bpfkern)
add_subdirectory(bpfsec)
