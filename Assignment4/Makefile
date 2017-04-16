CC = g++
CXX = $(CC)
DEBUG = -g -Wno-deprecated -fPIC

TARGETS = assignment4
C_SRCS = source/main.cpp \
		source/model.cpp \
		source/view.cpp \
		source/ply.cpp \
		source/controller.cpp 

CFLAGS = $(DEBUG) -I/usr/include -Iinclude -Wno-write-strings
LFLAGS = -g -L/usr/lib -lglut -lGL -lGLU -L./libs

CXXFLAGS = $(CFLAGS)

all: ${TARGETS}
	mv ${TARGETS} bin/

assignment4: ${C_SRCS}
	${CC} ${C_SRCS} -o assignment4 ${CFLAGS} ${LFLAGS}
	
clean:
	rm -f bin/*
