#!/bin/bash

gcc -Wall -c -fPIE -o build/interlope.o \
    interlope/src/main.c

ar rcs build/libinterlope.a build/interlope.o

rustc src/main.rs -o run -l interlope -L build/interlope
