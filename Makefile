BUILD=build
SRC=src
VENDOR=vendor

all:
	mkdir -p build
	gcc -I${VENDOR}/include -L ${VENDOR} \
			-lcglm \
			-lglfw \
		-lm -lGL -lGLEW \
		-o ${BUILD}/interlope\
			${SRC}/main.c \
			\
			${SRC}/system/file.c \
			${SRC}/system/logging.c \
			\
			${SRC}/engine/render.c \
			${SRC}/engine/time.c \
			${SRC}/engine/input.c \
			${SRC}/engine/camera.c \

	rm -rf ${BUILD}/shaders *.o ${SRC}/main
	cp -r ${SRC}/shaders ${BUILD}/shaders
run:
	cd ${BUILD} && \
	LD_LIBRARY_PATH=../${VENDOR} ./interlope && \
	cd ..
