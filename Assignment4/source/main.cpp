#include<stdio.h>
#include<model.h>
#include <view.h>
#include <controller.h>
#include <stdlib.h>
#include <iostream>
#include <ply.h>

static float x, y, z, fx, fy, fz;
static Model *m;
vector <Model *> m_vec;
vector <char [200]> t_name;
vector <char [200]> ply_name;

int main(int argc, char** argv)
{
	PlyFile* input;	
	m = new Model();
	View *v = new View(m);
	Controller *c = new Controller(m, v);
	
	if(argc != 2)
	{
		printf("Please run the program with Ply file and texture list as the parameter...\n");
		exit (0);
	}
		
	printf("Input file : %s\n", argv[1]);
	FILE * fp = fopen(argv[1], "r");
	
	input = read_ply(fp);

	if(input == NULL)
	{
		printf("Unable to Read Ply File\n");
		exit(0);
	}
	
	m->parse_ply(input);

	c->start(argc, argv);
	fclose(fp);
}