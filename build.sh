#!/bin/bash

set -eo pipefail


BUILD_DIR="./target/debug/build"
VENDOR_DIR="./interlope/vendor"
SRC_C="./interlope/src"
SRC_RS="./src"

echo "-> Cleaning build directory"
rm -rf ${BUILD_DIR}/*

echo "-> Compiling C sources"
gcc -c -fPIE -o ${BUILD_DIR}/interlope.o \
    -I ${VENDOR_DIR}/include -L ${VENDOR_DIR} \
    -lm \
    -lGL \
    -lGLEW \
    -lcglm \
    -lglfw \
    \
    ${SRC_C}/window.c

ar rcs ${BUILD_DIR}/libinterlope.a ${BUILD_DIR}/interlope.o

echo "-> Compiling Rust sources"
rustc -o ${BUILD_DIR}/interlope \
    -L ${BUILD_DIR} -l interlope \
    -L ${VENDOR_DIR} -l GL -l GLEW -l cglm -l glfw \
    \
    ${SRC_RS}/main.rs

echo "-> Done compiling"
