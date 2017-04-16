#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include <vector>
#ifndef WIN32
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#include <stdlib.h>
#include <GL/glut.h>

using namespace std;

class Texture
{
	private:		
		
	public:		
		Texture();
		
		vector <GLuint> texNames;
		vector <unsigned char *> textures;
		vector <unsigned int> tex_width;
		vector <unsigned int> tex_height;
};

#endif //__TEXTURE_H__
