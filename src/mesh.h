/*  by Javi Agenjo 2013 UPF  javi.agenjo@gmail.com
	The Mesh contains the info about how to render a mesh and also how to parse it from a file.
*/

#ifndef MESH_H
#define MESH_H

#include <vector>
#include "framework.h"
#include "extra/coldet/coldet.h"

class Shader;

class Mesh
{
public:
	std::vector< Vector3 > vertices; //here we store the vertices
	std::vector< Vector3 > normals;	 //here we store the normals
	std::vector< Vector2 > uvs;	 //here we store the texture coordinates
	std::vector< Vector4 > colors;	 //here we store colors by vertex

	Vector3 center;
	Vector3 halfSize;

	unsigned int vertices_vbo_id;
	unsigned int normals_vbo_id;
	unsigned int uvs_vbo_id;
	unsigned int colors_vbo_id;

	CollisionModel3D* collisionModel;
	CollisionModel3D* collisionBox;

	Mesh();
	Mesh( const Mesh& m );
	~Mesh();

	void clear();
	void render(int primitive);
	void render(int primitive, Shader* sh);

	void uploadToVRAM(); //store the mesh info in the GPU RAM so it renders faster

	void createPlane( float size );
	void createQuad(float center_x, float center_y, float w, float h, bool flip_uvs = false);

	bool loadMesh(char const* filename);
	bool loadASE(char const* filename);
	bool loadOBJ(char const* filename);
	bool loadBin(char const* filename);
	void writeBin(char const* bin_filename);
	void loadCollisionModels();
	Vector3 parseVector3(const char* text, const char separator);
	std::vector<std::string> tokenize(const std::string& source, const char* delimiters, bool process_strings = false);

};

#endif