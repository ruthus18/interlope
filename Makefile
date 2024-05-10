
all:
	mkdir -p build
	gcc -lSDL2 -lGL -lGLEW -lglut -o build/interlope\
		src/main.c \
		src/logging.c

	rm -rf build/shaders && cp -r src/shaders build/shaders
clean:
	rm -f *.o src/main
.PHONY: all clean

run:
	cd build && ./interlope && cd ..
