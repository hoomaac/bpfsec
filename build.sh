#!/bin/bash

BUILD_DIR="build"
BPFSEC_SRC_DIR="bpf"

function prepare_preqs() {
    local tools=linux-tools-generic
    dpkg -l ${tools} > /dev/null

    if [ $? -eq 1 ]; then
        echo "installing ${tools}"
        sudo apt install ${tools} linux-tools-$(uname -r)
    fi

    dpkg -l libbpf-dev > /dev/null
    [ $? -eq 1 ] && sudo apt install libbpf-dev

    # create vmlinux header file using bpftool
    if [ ! -f ${BPFSEC_SRC_DIR}/vmlinux.h ]; then
        bpftool btf dump file /sys/kernel/btf/vmlinux format c > ${BPFSEC_SRC_DIR}/vmlinux.h
    fi

    make -C libbpf/src BUILD_STATIC_ONLY=1 OBJDIR=$(pwd)/${BUILD_DIR}/libbpf

    local libbpf_include_dir="libbpf/include/libbpf"
    [ ! -d ${libbpf_include_dir} ] && mkdir ${libbpf_include_dir}; true;
    cp libbpf/src/*.h ${libbpf_include_dir} 
}

function build() {
    local build_type=Debug
    if [ "$1" == "debug" ] || [ "$1" == "Debug" ]; then
        [ ! -d build-debug ] && mkdir build-debug || true;
        BUILD_DIR=${BUILD_DIR}-debug
    else
        [ ! -d build-release ] && mkdir build-release || true;
        BUILD_DIR=${BUILD_DIR}-release
        build_type=release
    fi

    prepare_preqs

    cd ${BUILD_DIR} && cmake -G Ninja -DCMAKE_BUILD_TYPE=${build_type} .. && ninja
}

function clean() {
     if [ "$1" == "debug" ] || [ "$1" == "Debug" ]; then
        [ -d build-debug ] && rm -rf build-debug || echo "Nothing to be cleaned ;-)";
    else
        [ -d build-release ] && rm -rf build-release || echo "Nothing to be cleaned ;-)";
    fi
}

function menu() {
    PS3='>> '
    options=("build debug" "build release" "clean debug" "clean release" "exit")
    select opt in "${options[@]}"
    do
        case $opt in
            "build debug")
                build debug
                break
                ;;
            "build release")
                build release
                break
                ;;
            "clean debug")
                clean debug
                break
                ;;
            "clean release")
                clean release
                break
                ;;
            "exit")
                break
                ;;
            *) echo "invalid option $REPLY" 
                break
            ;;
        esac
    done
}

cat << END
  ____         __ _____           
 |  _ \       / _/ ____|          
 | |_) |_ __ | || (___   ___  ___ 
 |  _ <| '_ \|  _\___ \ / _ \/ __|
 | |_) | |_) | | ____) |  __/ (__ 
 |____/| .__/|_||_____/ \___|\___|
       | |                        
       |_|                        

END

menu