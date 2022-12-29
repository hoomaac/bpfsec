cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME "BpfProcessMonitor")

project(${PROJECT_NAME})

set(SOURCES
    bpfsec_process_mon.c)

find_program(LLC NAMES llc PATHS /usr/lib/llvm-14/bin/)
find_program(BPFTOOL NAMES bpftool PATHS /usr/sbin/)

if(NOT LLC)
    message(FATAL_ERROR "llvm static compiler not found!")
endif()

if(NOT BPFTOOL)
    message(FATAL_ERROR "bpftool not found!")
endif()

foreach(c_file ${SOURCES})
    file(REAL_PATH ${c_file} full_file_path)
    get_filename_component(filename ${full_file_path} NAME_WLE)

    # read more about bpftool skeleton: https://www.mankier.com/8/bpftool-gen
    execute_process(
        COMMAND bash -c "clang -g -O2 -I/usr/include/x86_64-linux-gnu -I/${CMAKE_SOURCE_DIR}/libbpf/include -D__TARGET_ARCH_x86 \
        -target bpf -emit-llvm -c ${full_file_path} -o - | ${LLC} -march=bpf -mcpu=v2 -filetype=obj -o ${CMAKE_CURRENT_BINARY_DIR}/${c_file}.o \
        && ${BPFTOOL} gen skeleton ${c_file}.o > ${CMAKE_CURRENT_SOURCE_DIR}/${filename}_skel.h"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    )

    list(APPEND OBJECTS ${CMAKE_CURRENT_BINARY_DIR}/${c_file}.o)
endforeach()

add_library(${PROJECT_NAME} OBJECT ${OBJECTS})
