#ifndef __CONTROLLER_H__
#define __CONTROLLER_H__

#include <defines.h>
#ifndef WIN32
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <stdlib.h>
#include <GL/glut.h>
#include <model.h>
#include <view.h>

class Controller
{
	private:
		Model *mdl;
		View  *vw;

		static Controller *ctrlInstance;
		
		int isTrans;
		GLfloat transx, transy;
		
	public:
		Controller(Model *m, View *v);
		void start(int argc, char** argv);
		void mouse(int button, int state, int x, int y);
		void keyPressed (unsigned char key, int x, int y);
		void specialKeyPressed (int key, int x, int y);
		
		static void mouseWrapper(int button, int state, int x, int y);
		static void keyPressedWrapper (unsigned char key, int x, int y);
		static void specialKeyWrapper (int key, int x, int y);
};

#endif //__CONTROLLER_H__