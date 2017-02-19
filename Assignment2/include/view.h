#ifndef __VIEW_H__
#define __VIEW_H__

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <GL/glut.h>

#include <model.h>


enum
{
	RESET,
	PICK_POINT,
};


class View
{
	private:
		Model *mdl;

		static View *viewInstance;

		

		
	public:
		View(Model *m);
		void start(int argc, char** argv);
		void init();
		void drawAxes();
		void drawBoundingBox();
		void drawFilledCircle();
		void drawTriangleMesh();
		static void displayWrapper();
		static void reshapeWrapper(int w, int h);
		
		
		virtual void display();
		virtual void reshape(int w, int h);
		void pickpoint(GLfloat *mouse_x, GLfloat *mouse_y);
};

#endif //__VIEW_H__