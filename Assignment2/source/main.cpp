#include<stdio.h>
#include<model.h>
#include <view.h>
#include <controller.h>
#include <stdlib.h>
#include <iostream>
#include <rply.h>
#include <ply.h>

static float x, y, z, fx, fy, fz;
static Model *m;

static int vertex_cb(p_ply_argument argument)
{
	m->vertex_cb(argument);
}
static int face_cb(p_ply_argument argument)
{
	m->face_cb(argument);
}


int main(int argc, char** argv)
{
	PlyFile* input;	
	m = new Model();
	View *v = new View(m);
	Controller *c = new Controller(m, v);
	
	if(argc != 2)
	{
		printf("Please run the program with relative path of ply file as the parameter...\n");
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
	
#if 1
	m->parse_ply(input);
	fclose(fp);

#else

	p_ply ply = ply_open(argv[1], NULL, 0, NULL);
    if (!ply) return 1;
    if (!ply_read_header(ply)) return 1;
	
	ply_set_read_cb(ply, "vertex", "x", vertex_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "y", vertex_cb, NULL, 0);
    ply_set_read_cb(ply, "vertex", "z", vertex_cb, NULL, 1);
	
	ply_set_read_cb(ply, "face", "vertex_indices", face_cb, NULL, 0);
    if (!ply_read(ply)) return 1;
    ply_close(ply);
#endif

	m->calculateTriangles();
	m->normalizeTriangles();
	m->calculateNormals();
	m->calculateIncircles();
	
	c->start(argc, argv);
}