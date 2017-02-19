#ifndef __MODEL_H__
#define __MODEL_H__

#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <vector>
#include <rply.h>
#include <ply.h>
#include <defines.h>

using namespace std;

static char* string_list[] = {
	"x", "y", "z", "nx", "ny", "nz", "vertex_indices"
};

class Model
{
	private:		
		vector <GLfloat> verts;
		vector <GLfloat> faces;
		
		
		
	public:		
		Model();
		
		void vertex_cb(p_ply_argument argument);
		void face_cb(p_ply_argument argument);
		void calculateTriangles();
		void calculateNormals();
		void normalizeTriangles();
		void calculateIncircles();
		void rotateByQuat(GLfloat deg);
		void scaleBox(GLfloat box_x, GLfloat box_y, GLfloat box_z);
		void parse_ply(PlyFile * input);
		
		vector <GLfloat> triangles;
		vector <GLfloat> normals;
		vector <GLfloat> incenter;
		vector <GLfloat> inradius;
		
		GLfloat box[8][3];
		long numTriangles;
		GLfloat xmax, ymax, zmax, xmin, ymin, zmin, xmid, ymid, zmid, xlen, ylen, zlen;
		int lights, colors, splats, mesh;
		GLfloat scalex, scaley, scalez;
		GLfloat transx, transy;
		GLfloat spin;
		GLfloat alpha;
		GLfloat Quat[16];
		GLfloat xi, yi, zi;
};

#endif //__MODEL_H__