
all:
	mkdir -p build
	gcc src/main.c -lSDL2 -lGL -lGLEW -lglut -o build/interlope
	rm -rf build/shaders && cp -r src/shaders build/shaders
clean:
	rm -f *.o src/main
.PHONY: all clean

run:
	cd build && ./interlope && cd ..
