#include <stdio.h>
#include <model.h>
#include <math.h>

Model::Model()
{
	lights = 0;
	colors = 0;
	scalex = scaley = scalez = 1;
	
	dr_mode = DRAW_MESH;
	transx = transy = 0;
	alpha = 1;
	
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
	
	xi = yi = zi = 0.01;
	rotateByQuat(0);
}

Model::~Model()
{
	verts.clear();
	faces.clear();
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

void Model::update_minmax(GLfloat x, GLfloat y, GLfloat z)
{
	v_max.x = (x>v_max.x)?x:v_max.x;
	v_max.y = (y>v_max.y)?y:v_max.y;
	v_max.z = (z>v_max.z)?z:v_max.z;

	v_min.x = (x<v_min.x)?x:v_min.x;
	v_min.y = (y<v_min.y)?y:v_min.y;
	v_min.z = (z<v_min.z)?z:v_min.z;
}

void Model::set_len()
{
	v_len.x = v_max.x - v_min.x;
	v_len.y = v_max.y - v_min.y;
	v_len.z = v_max.z - v_min.z;
}

void Model::set_mid()
{
	v_mid.x = (v_max.x + v_min.x)/2;
	v_mid.y = (v_max.y + v_min.y)/2;
	v_mid.z = (v_max.z + v_min.z)/2;
}

void Model::calculateTriangles()
{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	S_TRIANGLE curr_tri;
	int tri_vertex[3];
	numTriangles = 0;
	
	for(int i = 0; i < faces.size(); i++)
	{
		for(int j = 0;j<3;j++)
		{
			tri_vertex[0] = faces[i].vertices[j];
			x = curr_tri.verts[j].x = verts[tri_vertex[0]].x;
			y = curr_tri.verts[j].y = verts[tri_vertex[0]].y;
			z = curr_tri.verts[j].z = verts[tri_vertex[0]].z;

			update_minmax(x, y, z);
		}
		triangles.push_back(curr_tri);
	}
	set_len();
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
	set_mid();

	GLfloat max = v_len.x;
	max = (max > v_len.y)?(max > v_len.z)?max:v_len.z:v_len.y;
		
	scaleBox(v_len.x/max, v_len.y/max, v_len.z/max);
	
	for(int i = 0; i < triangles.size(); i++)
	{
		S_TRIANGLE *curr_tri;
		
		curr_tri = &(triangles[i]);

		for(int j = 0;j<3;j++)
		{
			GLfloat x = curr_tri->verts[j].x;
			GLfloat y = curr_tri->verts[j].y;
			GLfloat z = curr_tri->verts[j].z;

			//Normalize the vertices
			curr_tri->verts[j].x = 2 * (x - v_mid.x) / max;
			curr_tri->verts[j].y = 2 * (y - v_mid.y) / max;
			curr_tri->verts[j].z = 2 * (z - v_mid.z) / max;
		}
	}
}

void Model::calculateNormals()
{
	ASMT_NORMAL curr_norm;
	normals.clear();
	for(int i = 0; i < triangles.size(); i++)
	{
		S_TRIANGLE curr_tri = triangles[i];

		GLfloat coord1[3] = {curr_tri.verts[0].x, curr_tri.verts[0].y, curr_tri.verts[0].z};
		GLfloat coord2[3] = {curr_tri.verts[1].x, curr_tri.verts[1].y, curr_tri.verts[1].z};
		GLfloat coord3[3] = {curr_tri.verts[2].x, curr_tri.verts[2].y, curr_tri.verts[2].z};

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
		curr_norm.nx = vr[0]/val;
		curr_norm.ny = vr[1]/val;
		curr_norm.nz = vr[2]/val;
 
		normals.push_back(curr_norm);
	}
}

void Model::calculateIncircles()
{
	GLfloat a, b, c, p, x, y, z, s, Area;
	S_INCENTRE curr_in;

	incenter.clear();
	inradius.clear();
	
	for(int i = 0; i < triangles.size(); i++)
	{
		S_TRIANGLE curr_tri = triangles[i];
		GLfloat A[3] = {curr_tri.verts[0].x, curr_tri.verts[0].y, curr_tri.verts[0].z};
		GLfloat B[3] = {curr_tri.verts[1].x, curr_tri.verts[1].y, curr_tri.verts[1].z};
		GLfloat C[3] = {curr_tri.verts[2].x, curr_tri.verts[2].y, curr_tri.verts[2].z};
		
		a = sqrt(pow(B[0] - C[0], 2) + pow(B[1] - C[1], 2) + pow(B[2] - C[2], 2));
		b = sqrt(pow(A[0] - C[0], 2) + pow(A[1] - C[1], 2) + pow(A[2] - C[2], 2));
		c = sqrt(pow(B[0] - A[0], 2) + pow(B[1] - A[1], 2) + pow(B[2] - A[2], 2));
		
		p = a + b + c;
		
		curr_in.x = ( (a * A[0]) + (b * B[0]) + (c * C[0]) ) / p;
		curr_in.y = ( (a * A[1]) + (b * B[1]) + (c * C[1]) ) / p;
		curr_in.z = ( (a * A[2]) + (b * B[2]) + (c * C[2]) ) / p;
		
		incenter.push_back(curr_in);

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
				verts.push_back(read_vert);
				//verts.push_back(read_vert.x);
				//verts.push_back(read_vert.y);
				//verts.push_back(read_vert.z);
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
				faces.push_back(read_face);
				//faces.push_back(read_face.vertices[0]);
				//faces.push_back(read_face.vertices[1]);
				//faces.push_back(read_face.vertices[2]);
			}
		}
		// who knows?
		else {
			fprintf(stderr, "unknown element type found: %s\n", element);
		}
	}
}
