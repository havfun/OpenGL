#include <stdio.h>
#include <view.h>
#include <math.h>

View *View::viewInstance = NULL;
		
		
View::View(Model *m)
{
	mdl = m;
	viewInstance = this;
}



void View::start(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (800, 800);
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("MVC OpenGL 3D rendering");
   init ();
   glutDisplayFunc(displayWrapper);
   glutReshapeFunc(reshapeWrapper);
}
		
void View::init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);

    GLfloat light_position[] = { 1.5, 1.5, 1.5, 1.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

}

void View::drawAxes()
{
	glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex2d(-3, 0);
    glVertex2d(3, 0);
    glVertex2d(0, -3);
    glVertex2d(0, 3);
    glEnd();
}

void View::drawBoundingBox()
{
	glBegin(GL_LINE_LOOP);
	glVertex3fv(&mdl->box[0][0]);
	glVertex3fv(&mdl->box[1][0]);
	glVertex3fv(&mdl->box[2][0]);
	glVertex3fv(&mdl->box[3][0]);
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	glVertex3fv(&mdl->box[4][0]);
	glVertex3fv(&mdl->box[5][0]);
	glVertex3fv(&mdl->box[6][0]);
	glVertex3fv(&mdl->box[7][0]);
	glEnd();
	
	glBegin(GL_LINES);
	glVertex3fv(&mdl->box[0][0]);
	glVertex3fv(&mdl->box[4][0]);
	glVertex3fv(&mdl->box[1][0]);
	glVertex3fv(&mdl->box[5][0]);
	glVertex3fv(&mdl->box[2][0]);
	glVertex3fv(&mdl->box[6][0]);
	glVertex3fv(&mdl->box[3][0]);
	glVertex3fv(&mdl->box[7][0]);
	glEnd();
}


void View::reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
	if (w <= h)
       glOrtho(-2, 2, -2*(GLfloat)h/(GLfloat)w, 2*(GLfloat)h/(GLfloat)w, -2, 2);
    else
       glOrtho(-2*(GLfloat)w/(GLfloat)h, 2*(GLfloat)w/(GLfloat)h, -2, 2, -2, 2);
				
				
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

}



void View::drawFilledCircle()
{
	int i, j, k, m, n;	
	GLfloat color[3] = {1, 0, 0};
	
	for(i = 0, j = 0, k = 0; i < mdl->triangles.size(); i += 9, j += 3, k++)
	{
		GLfloat nx, ny, nz, xa, ya, za, xb, yb, zb, r, x, y, z;
		
		x = mdl->incenter[j];
		y = mdl->incenter[j + 1];
		z = mdl->incenter[j + 2];
		
		r = mdl->alpha * mdl->inradius[k];
		
		glNormal3f(mdl->normals[j], mdl->normals[j + 1], mdl->normals[j + 2]);
		if(mdl->colors)
		{
			color[0] = mdl->normals[j]; 
			color[1] = mdl->normals[j + 1];
			color[2] = mdl->normals[j + 2];
			
			color[0] = color[0] < 0 ? -1 * color[0] : color[0];
			color[1] = color[1] < 0 ? -1 * color[1] : color[1];
			color[2] = color[2] < 0 ? -1 * color[2] : color[2];
			
			glColor3fv(color);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
		}
		else
		{
			glColor3fv(color);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
		}
		
		
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(x, y, z); // center of circle
			
				
		for(m = 1; m <= 3; m++) 
		{ 
			xa = mdl->triangles[i + (m - 1) * 3 + 0];
			ya = mdl->triangles[i + (m - 1) * 3 + 1];
			za = mdl->triangles[i + (m - 1) * 3 + 2];
			
			xb = mdl->triangles[i + (m % 3) * 3 + 0];
			yb = mdl->triangles[i + (m % 3) * 3 + 1];
			zb = mdl->triangles[i + (m % 3) * 3 + 2];
			
			
	
			for(n = 0; n <= 10; n++)
			{
				GLfloat alpha = (GLfloat)n / 10;
				GLfloat R ;
				
				nx = (1 - alpha) * xa + alpha * xb;
				ny = (1 - alpha) * ya + alpha * yb;
				nz = (1 - alpha) * za + alpha * zb;
				
				R = sqrt(pow(x-nx, 2) + pow(y-ny, 2) + pow(z-nz, 2));
				alpha = r/R;
				
				nx = (1 - alpha) * x + alpha * nx;
				ny = (1 - alpha) * y + alpha * ny;
				nz = (1 - alpha) * z + alpha * nz;
				

			
				glVertex3f(nx, ny, nz);
			}
		}
		glEnd();
	}
}		
		
void View::drawTriangleMesh()
{
	int j = 0;
	GLfloat color[3] = {1, 0, 0};
	for(int i = 0; i < mdl->triangles.size(); )
	{
		if(mdl->colors)
		{
			color[0] = mdl->normals[j]; 
			color[1] = mdl->normals[j + 1];
			color[2] = mdl->normals[j + 2];
			
			color[0] = color[0] < 0 ? -1 * color[0] : color[0];
			color[1] = color[1] < 0 ? -1 * color[1] : color[1];
			color[2] = color[2] < 0 ? -1 * color[2] : color[2];
			
			glColor3fv(color);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
		}
		else
		{
			glColor3fv(color);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
		}
			

		if(mdl->mesh)
			glBegin(GL_LINE_LOOP);
		else
			glBegin(GL_TRIANGLES);
		
		glNormal3f(mdl->normals[j], mdl->normals[j + 1], mdl->normals[j + 2]);
		glVertex3f(mdl->triangles[i], mdl->triangles[i + 1], mdl->triangles[i + 2]);
		glVertex3f(mdl->triangles[i + 3], mdl->triangles[i + 4], mdl->triangles[i + 5]);
		glVertex3f(mdl->triangles[i + 6], mdl->triangles[i + 7], mdl->triangles[i + 8]);
		glEnd();
		
		i += 9;
		j += 3;

	}
}


void View::display(void)
{
	
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	drawAxes();
	
	GLfloat color[3] = {1, 1, 1};
	glPointSize(10.0);
	glColor3fv(color);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
	glBegin(GL_POINTS);
	glVertex3f(1.5, 1.5, 0);
	glEnd();

	if(mdl->lights)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
	
	

	glPushMatrix();
	glMultMatrixf(mdl->Quat);
	
	glScalef(mdl->scalex, mdl->scaley, mdl->scalez);
	glTranslatef(mdl->transx, mdl->transy, 0.0);

	if(mdl->splats)
	{
		drawFilledCircle();
	}
	else
	{
		drawTriangleMesh();
	}
	
	drawBoundingBox();
	glPopMatrix();

	glutSwapBuffers();
}

void View::pickpoint(GLfloat *mouse_x, GLfloat *mouse_y)
{
	
	GLdouble x, y, z;
	GLdouble projection_matrix[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1} ;
	GLdouble modelview_matrix[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1} ;
	GLint viewport[4] = {0,0,0,0}; 
		
		
	glGetDoublev(GL_PROJECTION_MATRIX, projection_matrix) ;
	glGetIntegerv(GL_VIEWPORT, viewport) ;
	glGetDoublev(GL_MODELVIEW_MATRIX, modelview_matrix) ;
	gluUnProject(*mouse_x, *mouse_y, 0,
					modelview_matrix, projection_matrix, viewport,
					&x, &y, &z);
	*mouse_x = x;
	*mouse_y = -1 * y;

}

void View::displayWrapper()
{
	viewInstance->display();
}

void View::reshapeWrapper(int w, int h)
{
	viewInstance->reshape(w, h);
}
		
