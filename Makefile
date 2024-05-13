
all:
	mkdir -p build
	gcc -Ivendor/include -L vendor \
			-lcglm \
			-lglfw \
		-lm -lGL -lGLEW \
		-o build/interlope\
			src/main.c \
			src/utils_io.c

	rm -rf build/shaders && cp -r src/shaders build/shaders
clean:
	rm -f *.o src/main
.PHONY: all clean

run:
	cd build && \
	LD_LIBRARY_PATH=../vendor ./interlope && \
	cd ..
