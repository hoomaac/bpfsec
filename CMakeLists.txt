cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME "bpfsec-project")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# set compiler
set(CMAKE_C_COMPILER "/usr/bin/clang")
set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(${PROJECT_NAME})

set(ThirdPartyIncludeDir ${CMAKE_SOURCE_DIR}/thirdparty/include)

include(ExternalProject)
ExternalProject_Add(yamlcpp
    GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    PREFIX ${CMAKE_SOURCE_DIR}/thirdparty/yamlcpp
    CMAKE_ARGS -DYAML_BUILD_SHARED_LIBS=OFF -DYAML_CPP_INSTALL=ON -DCMAKE_INSTALL_LIBDIR=${CMAKE_BINARY_DIR}/yamlcpp
    -DCMAKE_INSTALL_INCLUDEDIR=${ThirdPartyIncludeDir}
    )

include_directories(${ThirdPartyIncludeDir})

add_subdirectory(bpfkern)
add_subdirectory(bpfsec)
