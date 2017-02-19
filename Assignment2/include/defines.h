#if !defined(__DEFINES_H__)
#define __DEFINES_H__

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <string.h>
#include <vector>

#define NO_OTHER_PROPS  -1

#define DONT_STORE_PROP  0
#define STORE_PROP       1

#define OTHER_PROP       0
#define NAMED_PROP       1

#define COLOR2ARR(color_st, arr) \
	{arr[0] = (color_st).r; arr[1] = (color_st).g; arr[2] = (color_st).b;}

typedef struct 
{
	GLfloat x;
	GLfloat y;
} GLPOINT;

typedef struct VERTEX {
	GLfloat x, y, z;
	GLfloat nx, ny, nz;
} S_VERTEX;

typedef struct NORMAL {
	GLfloat nx;
	GLfloat ny;
	GLfloat nz;
} S_NORMAL;

typedef struct FACE {
	unsigned int idx;
	unsigned int *vertices;
	//float nx, ny, nz;
	S_NORMAL face_norm;
} S_FACE;

typedef struct TRIANGLE {
	S_VERTEX verts[3];
	S_NORMAL norm;
} S_TRIANGLE;

typedef struct TRANSLATE {
	GLfloat x;
	GLfloat y;
	GLfloat z;
} S_TRANSLATE;

typedef struct SCALE {
	GLfloat x;
	GLfloat y;
	GLfloat z;
} S_SCALE;

typedef struct COLOR {
	GLfloat r;
	GLfloat g;
	GLfloat b;
	GLfloat a;
} S_COLOR;

typedef enum
{
	D_RESET,
	D_WINEGLASS,
	D_BUTTERFLY,
	D_CRANE,
	D_ROTATE,
	D_SCALE,
	D_TRANS,
	D_PLOT,
	D_DELETE,
	D_UPDATE
} CMD;

typedef enum
{
	DRAW_POINT,
	DRAW_POLYGON,
	DRAW_MESH,
	DRAW_SPLAT
} DRAW_TYPE;

typedef enum 
{
	BEZIER,
	LAGRANGE
} CURVE_TYPE;


#endif			//__DEFINES_H__