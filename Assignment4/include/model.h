#ifndef __MODEL_H__
#define __MODEL_H__

#include <defines.h>
#ifndef WIN32
#include <GL/gl.h>
#include <GL/glu.h>
#endif

#include <stdlib.h>
#include <GL/glut.h>
#include <vector>
#include <ply.h>

using namespace std;

static char* string_list[] = {
	"x", "y", "z", "nx", "ny", "nz", "vertex_indices"
};

class Model
{
	private:		
		vector <S_VERTEX> verts;
		vector <S_FACE> faces;
		
		S_VERTEX v_max, v_min, v_mid, v_len;
		int lights, colors, dr_mode;	//splats, mesh;
		
		void update_minmax(GLfloat x, GLfloat y, GLfloat z);
		void set_len();
		void set_mid();
	public:		
		Model();
		~Model();
		void calculateTriangles();
		void calculateNormals();
		void normalizeTriangles();
		void calculateIncircles();
		void rotateByQuat(GLfloat deg);
		void scaleBox(GLfloat box_x, GLfloat box_y, GLfloat box_z);
		void parse_ply(PlyFile * input);
		
		void set_light(int val) {lights = val;};
		int  get_light()        {return lights;};
		
		void set_color(int val) {colors = val;};
		int  get_color() {return colors;};

		void set_draw_mode(int val) {dr_mode = val;};
		int  get_draw_mode() {return dr_mode;};

		vector <S_TRIANGLE> triangles;
		
		vector <ASMT_NORMAL> normals;
		vector <S_INCENTRE> incenter;
		vector <GLfloat> inradius;
		
		GLfloat box[8][3];
		long numTriangles;
		GLfloat scalex, scaley, scalez;
		GLfloat transx, transy;
		GLfloat spin;
		GLfloat alpha;
		GLfloat Quat[16];
		GLfloat xi, yi, zi;
};

#endif //__MODEL_H__