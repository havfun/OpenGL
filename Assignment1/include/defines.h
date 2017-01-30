#if !defined(__RENDER2D_DEFINES__)
#define __RENDER2D_DEFINES__

typedef struct 
{
	GLfloat x;
	GLfloat y;
} CPOINT;

typedef enum
{
	RESET,
	WINEGLASS,
	BUTTERFLY,
	CRANE,
} CMD;

typedef enum 
{
	BEZIER,
	LAGRANGE
} CURVE_TYPE;
#endif