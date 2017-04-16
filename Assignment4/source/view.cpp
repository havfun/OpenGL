#include <stdio.h>
#include <view.h>
#include <math.h>

View *View::viewInstance = NULL;
		
		
View::View()
{
	// mdl = m;
	viewInstance = this;
}


void View::addModel(Model *m)
{
	mdl.push_back(m);
}

void View::create_model()
{
	int count = mdl.size();
	for(int i = 0;i<count;i++)
	{
		mdl[i]->calculateTriangles();
		mdl[i]->normalizeTriangles();
		mdl[i]->calculateNormals();
		mdl[i]->calculateIncircles();
	}
}

void View::start(int argc, char** argv)
{
	create_model();
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (800, 800);
	glutInitWindowPosition (100, 100);
	glutCreateWindow ("Assignment 2");
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
	int i, m, n;	
	GLfloat color[3] = {0, 1, 0};
	
	for(i = 0; i < mdl->triangles.size(); i++)
	{
		GLfloat nx, ny, nz, xa, ya, za, xb, yb, zb, r, x, y, z;
		
		
		x = mdl->incenter[i].x;
		y = mdl->incenter[i].y;
		z = mdl->incenter[i].z;
		
		r = mdl->alpha * mdl->inradius[i];
		
		glNormal3f(mdl->normals[i].nx, mdl->normals[i].ny, mdl->normals[i].nz);
		if(mdl->get_color())
		{
			color[0] = abs(mdl->normals[i].nx); 
			color[1] = abs(mdl->normals[i].ny);
			color[2] = abs(mdl->normals[i].nz);
			
			glColor3fv(color);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
		}
		else
		{
			glColor3fv(color);
			glMaterialfv(GL_FRONT, GL_DIFFUSE, color);
		}
		
		
		glBegin(GL_TRIANGLE_FAN);
		glVertex3f(x, y, z);
			
				
		for(m = 0; m < 3; m++) 
		{ 
			S_TRIANGLE curr_tri;
			curr_tri = mdl->triangles[i];

			xa = curr_tri.verts[m].x;
			ya = curr_tri.verts[m].y;
			za = curr_tri.verts[m].z;

			xb = curr_tri.verts[(m+1)%3].x;
			yb = curr_tri.verts[(m+1)%3].y;
			zb = curr_tri.verts[(m+1)%3].z;

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
	GLfloat color[3] = {0, 1, 0};
	for(int i = 0; i < mdl->triangles.size(); i++)
	{
		if(mdl->get_color())
		{
			color[0] = mdl->normals[j].nx; 
			color[1] = mdl->normals[j].ny;
			color[2] = mdl->normals[j].nz;
			
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
			

		if(DRAW_MESH == mdl->get_draw_mode())
			glBegin(GL_LINE_LOOP);
		else
			glBegin(GL_TRIANGLES);
		
		glNormal3f(mdl->normals[j].nx, mdl->normals[j].ny, mdl->normals[j].nz);

		S_TRIANGLE curr_tri = mdl->triangles[i];
		glVertex3f(curr_tri.verts[0].x, curr_tri.verts[0].y, curr_tri.verts[0].z);
		glVertex3f(curr_tri.verts[1].x, curr_tri.verts[1].y, curr_tri.verts[1].z);
		glVertex3f(curr_tri.verts[2].x, curr_tri.verts[2].y, curr_tri.verts[2].z);

		glEnd();
		
		j++;

	}
}


void View::display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	drawAxes();
	
	if(mdl->get_light())
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);

	glPushMatrix();
	glMultMatrixf(mdl->Quat);
	
	glScalef(mdl->scalex, mdl->scaley, mdl->scalez);
	glTranslatef(mdl->transx, mdl->transy, 0.0);

	switch(mdl->get_draw_mode())
	{
	case DRAW_SPLAT:
		drawFilledCircle();
		break;
	case DRAW_MESH:
	case DRAW_POLYGON:
		drawTriangleMesh();
		break;
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
		
