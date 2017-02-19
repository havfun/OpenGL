CC = g++
CXX = $(CC)
DEBUG = -g -Wno-deprecated -fPIC

TARGETS = mvc
C_SRCS = source/main.cpp \
		source/model.cpp \
		source/view.cpp \
		source/ply.cpp \
		source/controller.cpp 

CFLAGS = $(DEBUG) -I/usr/include -Iinclude
LFLAGS = -g -L/usr/lib -lglut -lGL -lGLU -L./libs -lrply

CXXFLAGS = $(CFLAGS)

all: ${TARGETS}
	mv ${TARGETS} bin/

mvc: ${C_SRCS}
	${CC} ${C_SRCS} -o mvc ${CFLAGS} ${LFLAGS}
	
clean:
	rm -f bin/*
