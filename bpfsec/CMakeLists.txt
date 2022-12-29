cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME "bpfsecd")

project(${PROJECT_NAME})

set(SOURCES
    main.cpp
    process_monitor.cpp)

find_library(LIBBPF libbpf.a PATHS ${CMAKE_BINARY_DIR}/libbpf)
find_library(LIBELF libelf.a PATHS /usr/lib/x86_64-linux-gnu)
find_library(LIBZ libz.a PATH /usr/lib/x86_64-linux-gnu)

if (NOT LIBELF)
    message(FATAL_ERROR "Libelf not found")
endif()

if (NOT LIBZ)
    message(FATAL_ERROR "zlib not found")
endif()

add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/libbpf/include/)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE
    ${LIBBPF}
    ${LIBELF}
    ${LIBZ}
    BpfProcessMonitor)
