LDLIBS=-lglut -lGLEW -lGL -lSDL2

all: hello
clean:
	rm -f *.o hello
.PHONY: all clean

run:
	make && ./hello
