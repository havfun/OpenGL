#include <stdio.h>
#include <controller.h>


Controller *Controller::ctrlInstance = NULL;


Controller::Controller(Model *m, View *v)
{
	mdl = m;
	vw = v;

	ctrlInstance = this;
	
	isTrans = 0;
	transx = transy = 0;
}

void Controller::start(int argc, char** argv)
{
	vw->start(argc, argv);
	glutKeyboardFunc(keyPressedWrapper);
	glutMouseFunc(mouseWrapper);
	glutSpecialFunc(specialKeyWrapper);
	glutMainLoop();
}

void Controller::mouse(int button, int state, int x, int y)
{
	switch (button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_UP)
			 {
				 GLfloat fx, fy;
				 fx = x;
				 fy = y;
				if(!isTrans)
				{
					vw->pickpoint(&fx, &fy);
					transx = fx;
					transy = fy;
					isTrans = 1;
				}
				else
				{
					isTrans = 0;
					vw->pickpoint(&fx, &fy);
					mdl->transx = fx - transx;
					mdl->transy = fy - transy;

					glutPostRedisplay();
				}
			 }
		break;
		
		case GLUT_RIGHT_BUTTON:
			if (state == GLUT_UP)
			 {
				 GLfloat fx, fy;
				 fx = x;
				 fy = y;
				 
					vw->pickpoint(&fx, &fy);
					mdl->xi = fx;
					mdl->yi = fy;
			 }
		break;
	}
}

void Controller::specialKeyPressed (int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_LEFT:
			mdl->rotateByQuat(-5);
			glutPostRedisplay();
			break;
			
		case GLUT_KEY_RIGHT:
			mdl->rotateByQuat(5);
			glutPostRedisplay();
			break;
			
		case GLUT_KEY_UP:
			mdl->rotateByQuat(5);
			glutPostRedisplay();
			break;
			
		case GLUT_KEY_DOWN:
			mdl->rotateByQuat(-5);
			glutPostRedisplay();
			break;
	}
}

void Controller::keyPressed (unsigned char key, int x, int y)
{
	switch(key)
	{
		case ' ':
				mdl->lights = 1 - mdl->lights;
				glutPostRedisplay();
				break;
				
		case 'c':
				mdl->colors = 1 - mdl->colors;
				glutPostRedisplay();
				break;
		
		case 's':
				mdl->splats = 1 - mdl->splats;
				glutPostRedisplay();
				break;
			
		case 'a':
				mdl->mesh = 1 - mdl->mesh;
				glutPostRedisplay();
				break;

		case 'm':
				mdl->alpha += 0.1;
				if(mdl->alpha > 1.1)
					mdl->alpha = 1.1;
				glutPostRedisplay();
				break;
				
				
		case 'n':
				mdl->alpha -= 0.1;
				if(mdl->alpha < 0.1)
					mdl->alpha = 0.1;
				glutPostRedisplay();
				break;
				
			
		case '+':
				mdl->scalex += 0.05;
				if(mdl->scalex > 2)
					mdl->scalex = 2;
				mdl->scaley += 0.05;
				if(mdl->scaley > 2)
					mdl->scaley = 2;
				mdl->scalez += 0.05;
				if(mdl->scalez > 2)
					mdl->scalez = 2;
				glutPostRedisplay();
		break;
	
		case '-':
				mdl->scalex -= 0.05;
				if(mdl->scalex < 0.2)
					mdl->scalex = 0.2;
				mdl->scaley -= 0.05;
				if(mdl->scaley < 0.2)
					mdl->scaley = 0.2;
				mdl->scalez -= 0.05;
				if(mdl->scalez < 0.2)
					mdl->scalez = 0.2;
				glutPostRedisplay();
	}
}

void Controller::mouseWrapper(int button, int state, int x, int y)
{
	ctrlInstance->mouse(button, state, x, y);
}

void Controller::keyPressedWrapper (unsigned char key, int x, int y)
{
	ctrlInstance->keyPressed(key, x, y);
}

void Controller::specialKeyWrapper (int key, int x, int y)
{
	ctrlInstance->specialKeyPressed(key, x, y);
}

