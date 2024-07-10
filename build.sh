#!/bin/bash
set -eo pipefail


BUILD_DIR="./target/debug/build"
VENDOR_DIR="./interlope/vendor"
SRC_C="./interlope/src"
SRC_RS="./src"

GCC="gcc -Wall -fPIE -I ${VENDOR_DIR}/include -L ${VENDOR_DIR} \
        -lm \
        -lGL \
        -lGLEW \
        -lcglm \
        -lglfw \
"

printf "\033[0;32m-> [1/4] Cleaning build directory\033[0;0m\n"
rm -rf ${BUILD_DIR}/*

printf "\033[0;32m-> [2/4] Compiling C sources\033[0;0m\n"
${GCC} -c ${SRC_C}/camera.c -o ${BUILD_DIR}/camera.o
${GCC} -c ${SRC_C}/file.c -o ${BUILD_DIR}/file.o
${GCC} -c ${SRC_C}/input.c -o ${BUILD_DIR}/input.o
${GCC} -c ${SRC_C}/logging.c -o ${BUILD_DIR}/logging.o
${GCC} -c ${SRC_C}/render.c -o ${BUILD_DIR}/render.o
${GCC} -c ${SRC_C}/sample.c -o ${BUILD_DIR}/sample.o
${GCC} -c ${SRC_C}/time.c -o ${BUILD_DIR}/time.o

ar rcs ${BUILD_DIR}/libinterlope.a \
    ${BUILD_DIR}/camera.o \
    ${BUILD_DIR}/file.o \
    ${BUILD_DIR}/input.o \
    ${BUILD_DIR}/logging.o \
    ${BUILD_DIR}/render.o \
    ${BUILD_DIR}/sample.o \
    ${BUILD_DIR}/time.o \

printf "\033[0;32m-> [3/4] Compiling Rust sources\033[0;0m\n"
rustc -o ${BUILD_DIR}/interlope \
    -L ${BUILD_DIR} -l interlope \
    -L ${VENDOR_DIR} -l GL -l GLEW -l cglm -l glfw \
    \
    ${SRC_RS}/main.rs

printf "\033[0;32m-> [4/4] Done compiling\033[0;0m\n"
