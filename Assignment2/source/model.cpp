#include <stdio.h>
#include <model.h>
#include <math.h>

Model::Model()
{
	lights = 0;
	colors = 0;
	scalex = scaley = scalez = 1;
	
	splats = 0;
	transx = transy = 0;
	alpha = 1;
	mesh = 1;
	
	spin = 0;
	
	box[0][0] = 1;
	box[0][1] = 1;
	box[0][2] = 1;
	box[1][0] = -1;
	box[1][1] = 1;
	box[1][2] = 1;
	box[2][0] = -1;
	box[2][1] = 1;
	box[2][2] = -1;
	box[3][0] = 1;
	box[3][1] = 1;
	box[3][2] = -1;
	box[4][0] = 1;
	box[4][1] = -1;
	box[4][2] = 1;
	box[5][0] = -1;
	box[5][1] = -1;
	box[5][2] = 1;
	box[6][0] = -1;
	box[6][1] = -1;
	box[6][2] = -1;
	box[7][0] = 1;
	box[7][1] = -1;
	box[7][2] = -1;
	

	
	xi = yi = zi = 0;
	rotateByQuat(0);
}

void Model::rotateByQuat(GLfloat deg)
{
	//Find quaternion from given angle
	spin += deg;

	GLfloat half_angle = (spin * 0.5) * M_PI / 180.0;

	GLfloat x = xi * sin(half_angle);
	GLfloat y = yi * sin(half_angle);
	GLfloat z = zi * sin(half_angle);
	GLfloat w = cos(half_angle);
	
	GLfloat n = sqrt(x*x + y*y + z*z + w*w);
	x /= n;
	y /= n;
	z /= n;
	w /= n;
	
	//Construct matrix from quaternion
	GLfloat x2 = 2 * x * x;
	GLfloat y2 = 2 * y * y;
	GLfloat z2 = 2 * z * z;
	GLfloat w2 = 2 * w * w;
	GLfloat xy = 2 * x * y;
	GLfloat xz = 2 * x * z;
	GLfloat xw = 2 * x * w;
	GLfloat yz = 2 * y * z;
	GLfloat yw = 2 * y * w;
	GLfloat zw = 2 * z * w;
	
	Quat[0] = 1 - y2 - z2;
	Quat[1] = xy - zw;
	Quat[2] = xz + yw;
	Quat[3] = 0;
	Quat[4] = xy + zw;
	Quat[5] = 1 - x2 - z2; 
	Quat[6] = yz - xw;
	Quat[7] = 0;
	Quat[8] = xz - yw;
	Quat[9] = yz + xw; 
	Quat[10] = 1 - x2 - y2;
	Quat[11] = 0;
	Quat[12] = Quat[13] = Quat[14] = 0;
	Quat[15] = 1;

}


void Model::vertex_cb(p_ply_argument argument) {
	long eol;
    ply_get_argument_user_data(argument, NULL, &eol);
	verts.push_back(ply_get_argument_value(argument));
    return;
}

void Model::face_cb(p_ply_argument argument) {
	long length, value_index;
    ply_get_argument_property(argument, NULL, &length, &value_index);
    switch (value_index) {
        case 0:
        case 1:
			faces.push_back(ply_get_argument_value(argument));
            break;
        case 2:
			faces.push_back(ply_get_argument_value(argument));
            break;
        default:
            break;
    }
	
    return;
}

void Model::calculateTriangles()
{
	numTriangles = 0;
	
	for(int i = 0; i < faces.size(); i++)
	{
		GLfloat x = verts[faces[i] * 3];
		GLfloat y = verts[faces[i] * 3 + 1];
		GLfloat z = verts[faces[i] * 3 + 2];
	
		
		if(x > xmax)
			xmax = x;
		if(x < xmin)
			xmin = x;
		
		if(y > ymax)
			ymax = y;
		if(y < ymin)
			ymin = y;
		
		if(z > zmax)
			zmax = z;
		if(z < xmin)
			zmin = z;
		
		xlen = xmax - xmin;
		ylen = ymax - ymin;
		zlen = zmax - zmin;
		
		triangles.push_back(x);
		triangles.push_back(y);
		triangles.push_back(z);
	}
}

void Model::scaleBox(GLfloat box_x, GLfloat box_y, GLfloat box_z)
{
	for(int i = 0; i < 8; i++)
	{
		box[i][0] *= box_x;
		box[i][1] *= box_y;
		box[i][2] *= box_z;
	}
}

void Model::normalizeTriangles()
{
	
	xmid = (xmax + xmin) / 2;
	ymid = (ymax + ymin) / 2;
	zmid = (zmax + zmin) / 2;

	GLfloat max = xlen;
	max = max > ylen ? max : ylen;
	max = max > zlen ? max : zlen;
		
	if(max == xlen)
		scaleBox(1, ylen / xlen, zlen / xlen);
	else if(max == ylen)
		scaleBox(xlen / ylen, 1, zlen / ylen);
	else
		scaleBox(xlen / zlen, ylen / zlen, 1);
	
	for(int i = 0; i < triangles.size(); i += 3)
	{
		GLfloat x = triangles[i];
	    GLfloat y = triangles[i + 1];
        GLfloat z = triangles[i + 2];
		
		x -= xmid;
		y -= ymid;
		z -= zmid;
		
		//Normalize the vertices
		x = 2 * x / max;
		y = 2 * y / max;
		z = 2 * z / max;
		
		triangles[i] = x;
		triangles[i + 1] = y;
		triangles[i + 2] = z;
	}
}

void Model::calculateNormals()
{
	normals.clear();
	for(int i = 0; i < triangles.size(); )
	{
		GLfloat coord1[3] = {triangles[i], triangles[i + 1], triangles[i + 2]};
		GLfloat coord2[3] = {triangles[i + 3], triangles[i + 4], triangles[i + 5]};
		GLfloat coord3[3] = {triangles[i + 6], triangles[i + 7], triangles[i + 8]};
		
		i += 9;
		
		   /* calculate Vector1 and Vector2 */
		float va[3], vb[3], vr[3], val;
		va[0] = coord2[0] - coord1[0];
		va[1] = coord2[1] - coord1[1];
		va[2] = coord2[2] - coord1[2];
		
		vb[0] = coord3[0] - coord1[0];
		vb[1] = coord3[1] - coord1[1];
		vb[2] = coord3[2] - coord1[2];
		
		/* cross product */
		vr[0] = va[1] * vb[2] - vb[1] * va[2];
		vr[1] = vb[0] * va[2] - va[0] * vb[2];
		vr[2] = va[0] * vb[1] - vb[0] * va[1];
		
		/* normalization factor */
		val = sqrt( vr[0]*vr[0] + vr[1]*vr[1] + vr[2]*vr[2] );
 
		normals.push_back(vr[0]/val);
		normals.push_back(vr[1]/val);
		normals.push_back(vr[2]/val);
	}
}

void Model::calculateIncircles()
{
	GLfloat a, b, c, p, x, y, z, s, Area;

	incenter.clear();
	inradius.clear();
	
	for(int i = 0; i < triangles.size(); )
	{
		GLfloat A[3] = {triangles[i], triangles[i + 1], triangles[i + 2]};
		GLfloat B[3] = {triangles[i + 3], triangles[i + 4], triangles[i + 5]};
		GLfloat C[3] = {triangles[i + 6], triangles[i + 7], triangles[i + 8]};
		
		i += 9;
		
		a = sqrt(pow(B[0] - C[0], 2) + pow(B[1] - C[1], 2) + pow(B[2] - C[2], 2));
		b = sqrt(pow(A[0] - C[0], 2) + pow(A[1] - C[1], 2) + pow(A[2] - C[2], 2));
		c = sqrt(pow(B[0] - A[0], 2) + pow(B[1] - A[1], 2) + pow(B[2] - A[2], 2));
		
		p = a + b + c;
		
		x = ( (a * A[0]) + (b * B[0]) + (c * C[0]) ) / p;
		y = ( (a * A[1]) + (b * B[1]) + (c * C[1]) ) / p;
		z = ( (a * A[2]) + (b * B[2]) + (c * C[2]) ) / p;
		
		incenter.push_back(x);
		incenter.push_back(y);
		incenter.push_back(z);
		
		s = (a + b + c) / 2;
		Area = sqrt(s * (s - a) * (s - b) * (s - c) );
		inradius.push_back(2 * Area / p);
	}
	
	
}

void Model::parse_ply(PlyFile * input)
{
	S_VERTEX read_vert  = {0};
	FACE	read_face = {0};
	// go through the element types
	for(int i = 0; i < input->num_elem_types; i = i + 1) {
		int count;

		// setup the element for reading and get the element count
		char* element = setup_element_read_ply(input, i, &count);

		// get vertices
		if(strcmp("vertex", element) == 0) {

			// run through the properties and store them
			for(int j = 0; j < input->elems[i]->nprops; j = j + 1) {
				PlyProperty* property = input->elems[i]->props[j];
				PlyProperty setup;

				if(strcmp("x", property->name) == 0 &&
					property->is_list == PLY_SCALAR) {

						setup.name = string_list[0];
						setup.internal_type = Float32;
						setup.offset = offsetof(S_VERTEX, x);
						setup.count_internal = 0;
						setup.count_offset = 0;

						setup_property_ply(input, &setup);
				}
				else if(strcmp("y", property->name) == 0 &&
					property->is_list == PLY_SCALAR) {

						setup.name = string_list[1];
						setup.internal_type = Float32;
						setup.offset = offsetof(S_VERTEX, y);
						setup.count_internal = 0;
						setup.count_offset = 0;

						setup_property_ply(input, &setup);
				}
				else if(strcmp("z", property->name) == 0 &&
					property->is_list == PLY_SCALAR) {

						setup.name = string_list[2];
						setup.internal_type = Float32;
						setup.offset = offsetof(S_VERTEX, z);
						setup.count_internal = 0;
						setup.count_offset = 0;

						setup_property_ply(input, &setup);
				}
				else if(strcmp("nx", property->name) == 0 &&
					property->is_list == PLY_SCALAR) {

						setup.name = string_list[3];
						setup.internal_type = Float32;
						setup.offset = offsetof(S_VERTEX, nx);
						setup.count_internal = 0;
						setup.count_offset = 0;

						setup_property_ply(input, &setup);
				}
				else if(strcmp("ny", property->name) == 0 &&
					property->is_list == PLY_SCALAR) {

						setup.name = string_list[4];
						setup.internal_type = Float32;
						setup.offset = offsetof(S_VERTEX, ny);
						setup.count_internal = 0;
						setup.count_offset = 0;

						setup_property_ply(input, &setup);
				}
				else if(strcmp("nz", property->name) == 0 &&
					property->is_list == PLY_SCALAR) {

						setup.name = string_list[5];
						setup.internal_type = Float32;
						setup.offset = offsetof(S_VERTEX, nz);
						setup.count_internal = 0;
						setup.count_offset = 0;

						setup_property_ply(input, &setup);
				}
				// dunno what it is
				else {
					fprintf(stderr, "unknown property type found in %s: %s\n",
						element, property->name);
				}
			}

			// do this if you want to grab the other data
			// list_pointer = get_other_properties_ply
			//                (input, offsetof(Vertex, struct_pointer));

			// copy the data
			for(int j = 0; j < count; j = j + 1) {
				get_element_ply(input, (void*)(&read_vert));
				//verts.push_back(read_vert);
				verts.push_back(read_vert.x);
				verts.push_back(read_vert.y);
				verts.push_back(read_vert.z);
			}
		}
		// get faces
		else if(strcmp("face", element) == 0) {

			// run through the properties and store them
			for(int j = 0; j < input->elems[i]->nprops; j = j + 1) {
				PlyProperty* property = input->elems[i]->props[j];
				PlyProperty setup;

				if(strcmp("vertex_indices", property->name) == 0 &&
					property->is_list == PLY_LIST) {

						setup.name = string_list[6];
						setup.internal_type = Uint32;
						setup.offset = offsetof(FACE, vertices);
						setup.count_internal = Uint32;
						setup.count_offset = offsetof(FACE, idx);

						setup_property_ply(input, &setup);
				}
				else if(strcmp("nx", property->name) == 0 &&
					property->is_list == PLY_SCALAR) {

						setup.name = string_list[3];
						setup.internal_type = Float32;
						setup.offset = offsetof(FACE, face_norm.nx);
						setup.count_internal = 0;
						setup.count_offset = 0;

						setup_property_ply(input, &setup);
						//*facenormals = 1;
				}
				else if(strcmp("ny", property->name) == 0 &&
					property->is_list == PLY_SCALAR) {

						setup.name = string_list[4];
						setup.internal_type = Float32;
						setup.offset = offsetof(FACE, face_norm.ny);
						setup.count_internal = 0;
						setup.count_offset = 0;

						setup_property_ply(input, &setup);
						//*facenormals = 1;
				}
				else if(strcmp("nz", property->name) == 0 &&
					property->is_list == PLY_SCALAR) {

						setup.name = string_list[5];
						setup.internal_type = Float32;
						setup.offset = offsetof(FACE, face_norm.nz);
						setup.count_internal = 0;
						setup.count_offset = 0;

						setup_property_ply(input, &setup);
						//*facenormals = 1;
				}
				// dunno what it is
				else {
					fprintf(stderr, "unknown property type found in %s: %s\n",
						element, property->name);
				}
			}

			// do this if you want to grab the other data
			// list_pointer = get_other_properties_ply
			//                (input, offsetof(Face, struct_pointer));

			// copy the data
			for(int j = 0; j < count; j = j + 1) {
				get_element_ply(input, (void*)(&read_face));
				//faces.push_back(read_face);
				faces.push_back(read_face.vertices[0]);
				faces.push_back(read_face.vertices[1]);
				faces.push_back(read_face.vertices[2]);
			}
		}
		// who knows?
		else {
			fprintf(stderr, "unknown element type found: %s\n", element);
		}
	}
}