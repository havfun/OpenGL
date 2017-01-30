#include<GL/glut.h>
#include<math.h>
#include<stdio.h>
#include<stdlib.h>
#include<defines.h>
#include<wine_glass.h>
#include<butterfly.h>


static GLfloat spin = 0.0;
static GLfloat scalex = 1.0, scaley = 1.0;
static GLfloat transx = 0.0, transy = 0.0;
static GLint isTrans = 0;
static GLint option = 0;

void init(void) 
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void bezierCoefficients(int n,float *c)
{
	int k,i;
	for(k=0;k<=n;k++)
	{
		c[k]=1;
		for(i=(k+1);i<n;i++)
			c[k]*=i;
		for(i=1;i<n-k;i++)
			c[k]/=i;
	}
}

void disp_points()
{
	int i, j;
	int prev_idx;
	GLfloat x[2];

	glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT);
	glPointSize(2.0);
	glColor3f(0.0, 1.0, 0.0);

	prev_idx = 0;
	
	//glBegin(GL_LINE_STRIP);
	glBegin(GL_POINTS);

	for(j = 0; j < num_curve; j++)
	{
		for (i = prev_idx; i < prev_idx + curve_size[j]; i++) 
		{
			glVertex2f(curve[i].x, curve[i].y);
		}
		prev_idx = i;
	}   
	glEnd();
}

unsigned long long int fact(int n) {
	int c;
	unsigned long long int result = 1;

	for (c = 1; c <= n; c++)
		result = result*c;

	return result;
}

long double nCr(int n, int r) {
	int c;
	long double result = 1;
	int k = n - r;
	
	if(r < k)
	{
		int t = r;
		r = k;
		k = t;
	}

	for(c = r+1;c<=n;c++)
		result = result*c;
	for(c = 1;c<=k;c++)
		result = result/c;

	return result;
}

void disp_bz_curve(int idx)
{
	GLint k;
	GLfloat x, y, u, blend;
	int n_points;
	int start_idx;

	glColor3f(1.0,0.0,0.0);

	start_idx = 0;
	for(int i = 0;i<idx;i++)
	{
		start_idx += curve_size[i];
	}

	n_points = curve_size[idx];


	glBegin(GL_LINE_STRIP);

	for(u=0;u<1.0;u+=0.01)
	{
		x = 0;
		y = 0;
		for(k=0;k<n_points;k++)
		{
			blend = nCr(n_points, k) * pow(u,k) * pow(1-u,n_points-k);
			x += curve[k+start_idx].x*blend;
			y += curve[k+start_idx].y*blend;
		}

		blend = nCr(n_points, k) * pow(u,k) * pow(1-u,n_points-k);
		x += curve[0+start_idx].x*blend;
		y += curve[0+start_idx].y*blend;

		glVertex2f(x, y);
	}
	glEnd();
}

void print_usage()
{
	printf("Key Codes :\n");
	printf("\t1\t:Wine Glass");
	printf("\t2\t:Butterfly");
	printf("\t3\t:Crane");
	printf("\t Esc/Q/q\t: Quit Program\n");
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'r':
		cmd = ;
		break;
	case '1':
		// Draw Wine Glass
		break;
	case '2':
		// Draw Butterfly
		break;
	case '3':
		// Draw Crane
		break;

	case 'q':
	case 'Q':
	case 27:
		exit(0);
		break;
	default:
		printf("Invalid Key Pressed\n");
		print_usage();
		break;
	}
}

void display()
{

	//int cp[4][2]={{10,10},{100,200},{200,50},{300,300}};
	float c[40];
	int i, k;
	float x,y,u,blend;

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 0.0, 1.0);

	glTranslatef(transx, transy, 0.0);
	glScalef(scalex, scaley, 0.0);
	glRotatef(spin, 0.0, 0.0, 1.0); 


	disp_points();

	for(i=0;i<num_curve;i++)
	{
		disp_bz_curve(i);
	}

	glFlush();
	return;


}


void reshape(int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-400.0, 400.0, -400.0*(GLfloat)h/(GLfloat)w, 
		400.0*(GLfloat)h/(GLfloat)w, -400.0, 400.0);
	else
		glOrtho(-400.0*(GLfloat)w/(GLfloat)h, 400.0*(GLfloat)w/(GLfloat)h,
		-400.0, 400.0, -400.0, 400.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouse(int button, int state, int x, int y) 
{
	return;
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize (1000, 1000);
	glutInitWindowPosition (100, 100);

	glutCreateWindow("2DCurve");

	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc (keyboard);
	glutMouseFunc(mouse);

	glutMainLoop();
	return 0;

}
