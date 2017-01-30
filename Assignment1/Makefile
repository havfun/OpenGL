CC = g++
CXX = $(CC)
DEBUG = -g -Wno-deprecated -fPIC


TARGETS = render_2d
C_SRCS = ./source/render_2d.c
#OBJS = ${C_SRCS:.c=.o}

# release settings
#CFLAGS = -O2 -I/usr/include
#LFLAGS = -s -L/usr/lib

# debug settings
CFLAGS = $(DEBUG) $(DEFINES) -I/usr/include -I./include
LFLAGS = -g -L/usr/lib -lglut -lGL -lGLU -lXi -lXmu -lm

CXXFLAGS = $(CFLAGS)

all: ${TARGETS}
	mv ${TARGETS} bin/

double_teapot: double_teapot.c
	gcc double_teapot.c -o double_teapot ${LFLAGS}

double_spin: double.c
	gcc double.c -o double_spin ${LFLAGS}

sphere: sphere.c
	gcc sphere.c -o sphere ${LFLAGS}

lightsphere: lightsphere.c
	gcc lightsphere.c -o lightsphere ${LFLAGS}

planet: planet.c
	gcc planet.c -o planet ${LFLAGS}

pickpoint: pickpoint.c
	gcc pickpoint.c -o pickpoint ${LFLAGS}

timer: timer.c
	gcc timer.c -o timer ${LFLAGS}


render_2d: ./source/render_2d.cpp
	gcc ./source/render_2d.cpp -o render_2d ${LFLAGS} ${CFLAGS}

clean:
	rm -f bin/*



