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

GLfloat trans_st_x = 0, trans_st_y = 0;
void Controller::mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		GLfloat fx, fy;
		fx = x;
		fy = y;
		if (state == GLUT_DOWN) 
		{
			vw->pickpoint(&fx, &fy);
			trans_st_x = fx;
			trans_st_y = fy;
		}
		else if (state == GLUT_UP) 
		{
			vw->pickpoint(&fx, &fy);
			mdl->transx = fx - trans_st_x;
			mdl->transy = (fy - trans_st_y);
			glutPostRedisplay();
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
			mdl->spin = 0;
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
		break;

	case GLUT_KEY_RIGHT:
		mdl->rotateByQuat(5);
		break;

	case GLUT_KEY_UP:
		mdl->rotateByQuat(5);
		break;

	case GLUT_KEY_DOWN:
		mdl->rotateByQuat(-5);
		break;
	}
	glutPostRedisplay();
}

void Controller::keyPressed (unsigned char key, int x, int y)
{
	switch(key)
	{
	case 'l':
		mdl->set_light(!mdl->get_light());
		break;
	case 'c':
		mdl->set_color(!mdl->get_color());
		break;
	case 'a':
		mdl->set_draw_mode(DRAW_MESH);
		break;
	case 's':
		mdl->set_draw_mode(DRAW_SPLAT);
		break;
	case 'd':
		mdl->set_draw_mode(DRAW_POLYGON);
		break;
	case 'm':
		mdl->alpha += 0.1;
		if(mdl->alpha > 1.1)
			mdl->alpha = 1.1;
		break;
	case 'n':
		mdl->alpha -= 0.1;
		if(mdl->alpha < 0.1)
			mdl->alpha = 0.1;
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
		break;
	case 'q':
	case 27:
		exit(0);
		break;
	}
	glutPostRedisplay();
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

