#include "mesh.h"
#include <cassert>
#include "includes.h"
#include "shader.h"
#include "extra/textparser.h"
#include "utils.h"
#include <string>
#include <sys/stat.h>


Mesh::Mesh()
{
	vertices_vbo_id = 0;
	normals_vbo_id = 0;
	uvs_vbo_id = 0;
	colors_vbo_id = 0;
	collisionModel = NULL;
	collisionBox = NULL;
	center = halfSize = Vector3();
}

Mesh::Mesh( const Mesh& m )
{
	vertices = m.vertices;
	normals = m.normals;
	uvs = m.uvs;
	colors = m.colors;
	collisionModel = m.collisionModel;
	collisionBox = m.collisionBox;
	center = m.center;
	halfSize = m.halfSize;
}

Mesh::~Mesh()
{
	if(vertices_vbo_id) glDeleteBuffersARB(1, &vertices_vbo_id);
	if(normals_vbo_id) glDeleteBuffersARB(1, &normals_vbo_id);
	if(uvs_vbo_id) glDeleteBuffersARB(1, &uvs_vbo_id);
	if(colors_vbo_id) glDeleteBuffersARB(1, &colors_vbo_id);
}

void Mesh::clear()
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();
	collisionModel = NULL;
	collisionBox = NULL;
	center = Vector3();
	halfSize = Vector3();
}

void Mesh::render(int primitive)
{
	assert(vertices.size() && "No vertices in this mesh");

    glEnableClientState(GL_VERTEX_ARRAY);

	if(vertices_vbo_id)
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, vertices_vbo_id );
		glVertexPointer(3, GL_FLOAT, 0, NULL );
	}
	else
		glVertexPointer(3, GL_FLOAT, 0, &vertices[0] );

	if (normals.size())
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		if(normals_vbo_id)
		{
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, normals_vbo_id );
			glNormalPointer(GL_FLOAT, 0, NULL );
		}
		else
			glNormalPointer(GL_FLOAT, 0, &normals[0] );
	}

	if (uvs.size())
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		if(uvs_vbo_id)
		{
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, uvs_vbo_id );
			glTexCoordPointer(2, GL_FLOAT, 0, NULL );
		}
		else
			glTexCoordPointer(2, GL_FLOAT, 0, &uvs[0] );
	}

	if (colors.size())
	{
		glEnableClientState(GL_COLOR_ARRAY);
		if(colors_vbo_id)
		{
			glBindBufferARB( GL_ARRAY_BUFFER_ARB, colors_vbo_id );
			glColorPointer(4, GL_FLOAT, 0, NULL );
		}
		else
			glColorPointer(4, GL_FLOAT, 0, &colors[0] );
	}
    
	glDrawArrays(primitive, 0, (GLsizei)vertices.size() );
	glDisableClientState(GL_VERTEX_ARRAY);

	if (normals.size())
		glDisableClientState(GL_NORMAL_ARRAY);
	if (uvs.size())
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	if (colors.size())
		glDisableClientState(GL_COLOR_ARRAY);
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
}

void Mesh::render( int primitive, Shader* sh )
{
	if(!sh || !sh->compiled)
        return render(primitive);
    
	assert(vertices.size() && "No vertices in this mesh");

    int vertex_location = sh->getAttribLocation("a_vertex");
	assert(vertex_location != -1 && "No a_vertex found in shader");

	if(vertex_location == -1)
		return;

    glEnableVertexAttribArray(vertex_location);
	if(vertices_vbo_id)
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, vertices_vbo_id );
		glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, NULL );
	}
	else
		glVertexAttribPointer(vertex_location, 3, GL_FLOAT, GL_FALSE, 0, &vertices[0] );

	int normal_location = -1;
    if (normals.size())
    {
        normal_location = sh->getAttribLocation("a_normal");
        if(normal_location != -1)
        {
            glEnableVertexAttribArray(normal_location);
			if(normals_vbo_id)
			{
				glBindBufferARB( GL_ARRAY_BUFFER_ARB, normals_vbo_id );
	            glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 0, NULL );
			}
			else
	            glVertexAttribPointer(normal_location, 3, GL_FLOAT, GL_FALSE, 0, &normals[0] );
        }
    }
    
	int uv_location = -1;
	if (uvs.size())
	{
        uv_location = sh->getAttribLocation("a_uv");
        if(uv_location != -1)
        {
            glEnableVertexAttribArray(uv_location);
			if(uvs_vbo_id)
			{
				glBindBufferARB( GL_ARRAY_BUFFER_ARB, uvs_vbo_id );
	            glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, 0, NULL );
			}
			else
	            glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, 0, &uvs[0] );
        }
    }
    
	int color_location = -1;
	if (colors.size())
	{
        color_location = sh->getAttribLocation("a_color");
        if(color_location != -1)
        {
            glEnableVertexAttribArray(color_location);
			if(colors_vbo_id)
			{
				glBindBufferARB( GL_ARRAY_BUFFER_ARB, colors_vbo_id );
	            glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, 0, NULL );
			}
			else
	            glVertexAttribPointer(color_location, 4, GL_FLOAT, GL_FALSE, 0, &colors[0] );
        }
    }
    
	glDrawArrays(primitive, 0, (GLsizei)vertices.size() );

	glDisableVertexAttribArray( vertex_location );
	if(normal_location != -1) glDisableVertexAttribArray( normal_location );
	if(uv_location != -1) glDisableVertexAttribArray( uv_location );
	if(color_location != -1) glDisableVertexAttribArray( color_location );
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, 0 );
}

void Mesh::uploadToVRAM()
{
	if (glGenBuffersARB == 0)
	{
		std::cout << "Error: your graphics cards dont support VBOs. Sorry." << std::endl;
		exit(0);
	}

	//delete old
	if(vertices_vbo_id) glDeleteBuffersARB(1, &vertices_vbo_id);
	if(normals_vbo_id) glDeleteBuffersARB(1, &normals_vbo_id);
	if(uvs_vbo_id) glDeleteBuffersARB(1, &uvs_vbo_id);
	if(colors_vbo_id) glDeleteBuffersARB(1, &colors_vbo_id);

	glGenBuffersARB( 1, &vertices_vbo_id ); //generate one handler (id)
	glBindBufferARB( GL_ARRAY_BUFFER_ARB, vertices_vbo_id ); //bind the handler
	glBufferDataARB( GL_ARRAY_BUFFER_ARB, vertices.size() * 3 * sizeof(float), &vertices[0], GL_STATIC_DRAW_ARB ); //upload data

	if(normals.size())
	{
		glGenBuffersARB( 1, &normals_vbo_id); //generate one handler (id)
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, normals_vbo_id ); //bind the handler
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, normals.size() * 3 * sizeof(float), &normals[0], GL_STATIC_DRAW_ARB ); //upload data
	}

	if(uvs.size())
	{
		glGenBuffersARB( 1, &uvs_vbo_id); //generate one handler (id)
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, uvs_vbo_id ); //bind the handler
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, uvs.size() * 2 * sizeof(float), &uvs[0], GL_STATIC_DRAW_ARB ); //upload data
	}

	if(colors.size())
	{
		glGenBuffersARB( 1, &colors_vbo_id); //generate one handler (id)
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, colors_vbo_id ); //bind the handler
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, uvs.size() * 4 * sizeof(float), &colors[0], GL_STATIC_DRAW_ARB ); //upload data
	}

}

void Mesh::createQuad(float center_x, float center_y, float w, float h, bool flip_uvs )
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();

	//create six vertices (3 for upperleft triangle and 3 for lowerright)
	vertices.push_back( Vector3( center_x + w*0.5f, center_y + h*0.5f, 0.0f ) );
	vertices.push_back( Vector3( center_x - w*0.5f, center_y - h*0.5f, 0.0f ) );
	vertices.push_back( Vector3( center_x + w*0.5f, center_y - h*0.5f, 0.0f ) );
	vertices.push_back( Vector3( center_x - w*0.5f, center_y + h*0.5f, 0.0f ) );
	vertices.push_back( Vector3( center_x - w*0.5f, center_y - h*0.5f, 0.0f ) );
	vertices.push_back( Vector3( center_x + w*0.5f, center_y + h*0.5f, 0.0f ) );

	//texture coordinates
	uvs.push_back( Vector2(1.0f,flip_uvs ? 0.0f : 1.0f) );
	uvs.push_back( Vector2(0.0f,flip_uvs ? 1.0f : 0.0f) );
	uvs.push_back( Vector2(1.0f,flip_uvs ? 1.0f : 0.0f) );
	uvs.push_back( Vector2(0.0f,flip_uvs ? 0.0f : 1.0f) );
	uvs.push_back( Vector2(0.0f,flip_uvs ? 1.0f : 0.0f) );
	uvs.push_back( Vector2(1.0f,flip_uvs ? 0.0f : 1.0f) );

	//all of them have the same normal
	normals.push_back( Vector3(0.0f,0.0f,1.0f) );
	normals.push_back( Vector3(0.0f,0.0f,1.0f) );
	normals.push_back( Vector3(0.0f,0.0f,1.0f) );
	normals.push_back( Vector3(0.0f,0.0f,1.0f) );
	normals.push_back( Vector3(0.0f,0.0f,1.0f) );
	normals.push_back( Vector3(0.0f,0.0f,1.0f) );
}


void Mesh::createPlane(float size)
{
	vertices.clear();
	normals.clear();
	uvs.clear();
	colors.clear();

	//create six vertices (3 for upperleft triangle and 3 for lowerright)
	vertices.push_back( Vector3(size,0,size) );
	vertices.push_back( Vector3(size,0,-size) );
	vertices.push_back( Vector3(-size,0,-size) );
	vertices.push_back( Vector3(-size,0,size) );
	vertices.push_back( Vector3(size,0,size) );
	vertices.push_back( Vector3(-size,0,-size) );

	
	//all of them have the same normal
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );
	normals.push_back( Vector3(0,1,0) );

	//texture coordinates
	uvs.push_back( Vector2(1,1) );
	uvs.push_back( Vector2(1,0) );
	uvs.push_back( Vector2(0,0) );
	uvs.push_back( Vector2(0,1) );
	uvs.push_back( Vector2(1,1) );
	uvs.push_back( Vector2(0,0) );
	
}

/*
Carregar fitxer
Parsing (obtenir vèrtexs i superficies i normals
Guardar a disc la mesh parsejada (.bin)
--> crear un struct per a guardar al fitxer bin el nombre de vertexs
*/

bool Mesh::loadMesh(char const* filename) {
	std::string meshesPath = "data/meshes/";
	meshesPath.append(filename);

	std::string asePath = meshesPath;
	asePath.append(".ase");

	std::string objPath = meshesPath;
	objPath.append(".obj");

	if (FILE *file = fopen(asePath.c_str(), "r")) {
		fclose(file);
		return loadASE(asePath.c_str());
	}
	else if (FILE *file = fopen(objPath.c_str(), "r")) {
		fclose(file);
		return loadOBJ(objPath.c_str());
	}
}

bool Mesh::loadASE(char const* filename)
{	
	std::string binPath = filename;
	binPath.append(".bin");
		
	if (loadBin(binPath.c_str())) return true;

	std::cout << "Loading Mesh from ASE..." << std::endl;

	TextParser t;
	if(t.create(filename) == false)return false;
	t.seek("*MESH_NUMVERTEX");
	int numVertexs = t.getint();
	t.seek("*MESH_NUMFACES");
	int numFaces = t.getint();
	int totalVertexs = numFaces * 3;

	//Vertexs
	std::vector< Vector3 > uniqueVertex;
	uniqueVertex.resize(numVertexs);

	vertices.clear();
	vertices.resize(totalVertexs);

	for (int i = 0; i < numVertexs; i++)
	{
		t.seek("*MESH_VERTEX");
		t.getint();
		float x = t.getfloat();
		float z = -t.getfloat();
		float y = t.getfloat();
		uniqueVertex[i] = Vector3(x, y, z);
	}

	for (int i = 0; i < totalVertexs; i+=3)
	{
		t.seek("*MESH_FACE");
		t.getword();
		t.getword();
		int A = t.getint();
		t.getword();
		int B = t.getint();
		t.getword();
		int C = t.getint();
		vertices[i] = (uniqueVertex[A]);
		vertices[i+1] = (uniqueVertex[B]);
		vertices[i+2] = (uniqueVertex[C]);		
	}

	//Uv
	t.seek("*MESH_NUMTVERTEX");
	int numUvVertexs = t.getint();
	std::vector< Vector2 > uniqueUvs;
	uniqueUvs.resize(numUvVertexs);

	for (int i = 0; i < numUvVertexs; i++) {
		t.seek("*MESH_TVERT");
		t.getint();
		float x = t.getfloat();
		float y = t.getfloat();
		uniqueUvs[i] = Vector2(x, y);
	}

	t.seek("*MESH_NUMTVFACES");
	int totalUvVertexs = t.getint() * 3;
	uvs.resize(totalUvVertexs);

	for (int i = 0; i < totalUvVertexs; i += 3)
	{
		t.seek("*MESH_TFACE");
		t.getint();
		int A = t.getint();
		int B = t.getint();
		int C = t.getint();
		uvs[i] = (uniqueUvs[A]);
		uvs[i + 1] = (uniqueUvs[B]);
		uvs[i + 2] = (uniqueUvs[C]);
	}
	
	//Normals
	normals.clear();
	normals.resize(totalVertexs);

	for (int i = 0; i < totalVertexs; i++) 
	{
		t.seek("*MESH_VERTEXNORMAL");
		t.getint();
		float x = t.getfloat();
		float z = -t.getfloat();
		float y = t.getfloat();
		normals[i] = Vector3(x, y, z);
	}

	std::cout << "ASE loaded." << std::endl;
	
	writeBin(binPath.c_str());

	loadCollisionModels();

	return true;
}

bool Mesh::loadOBJ(char const* filename) {
	
	std::string binPath = filename;
	binPath.append(".bin");

	if (loadBin(binPath.c_str())) return true;

	std::cout << "Loading Mesh from OBJ..." << std::endl;

	struct stat stbuffer;

	FILE* f = fopen(filename, "rb");
	if (f == NULL)
	{
		std::cerr << "File not found: " << filename << std::endl;
		return false;
	}

	stat(filename, &stbuffer);

	unsigned int size = stbuffer.st_size;
	char* data = new char[size + 1];
	fread(data, size, 1, f);
	fclose(f);
	data[size] = 0;

	char* pos = data;
	char line[500];
	int i = 0;

	std::vector<Vector3> indexed_positions;
	std::vector<Vector3> indexed_normals;
	std::vector<Vector2> indexed_uvs;

	const float max_float = 10000000;
	const float min_float = -10000000;

	unsigned int vertex_i = 0;

	//parse file
	while (*pos != 0)
	{
		if (*pos == '\n') pos++;
		if (*pos == '\r') pos++;

		//read one line
		i = 0;
		while (i < 500 && pos[i] != '\n' && pos[i] != '\r' && pos[i] != 0) i++;
		memcpy(line, pos, i);
		line[i] = 0;
		pos = pos + i;

		//std::cout << "Line: \"" << line << "\"" << std::endl;
		if (*line == '#' || *line == 0) continue; //comment

												  //tokenize line
		std::vector<std::string> tokens = tokenize(line, " ");

		if (tokens.empty()) continue;
		
		if (tokens[0] == "v" && tokens.size() == 4)
		{
			Vector3 v(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()));
			indexed_positions.push_back(v);
		}
		else if (tokens[0] == "vt" && (tokens.size() == 4 || tokens.size() == 3))
		{
			Vector2 v(atof(tokens[1].c_str()), atof(tokens[2].c_str()));
			indexed_uvs.push_back(v);
		}
		else if (tokens[0] == "vn" && tokens.size() == 4)
		{
			Vector3 v(atof(tokens[1].c_str()), atof(tokens[2].c_str()), atof(tokens[3].c_str()));
			indexed_normals.push_back(v);
		}
		else if (tokens[0] == "s") //surface? it appears one time before the faces
		{
			//process mesh
			if (uvs.size() == 0 && indexed_uvs.size())
				uvs.resize(1);
		}
		else if (tokens[0] == "f" && tokens.size() >= 4)
		{
			Vector3 v1, v2, v3;
			v1 = parseVector3(tokens[1].c_str(), '/');

			for (int iPoly = 2; iPoly < tokens.size() - 1; iPoly++)
			{
				v2 = parseVector3(tokens[iPoly].c_str(), '/');
				v3 = parseVector3(tokens[iPoly + 1].c_str(), '/');

				vertices.push_back(indexed_positions[unsigned int(v1.x) - 1]);
				vertices.push_back(indexed_positions[unsigned int(v2.x) - 1]);
				vertices.push_back(indexed_positions[unsigned int(v3.x) - 1]);
				//triangles.push_back( VECTOR_INDICES_TYPE(vertex_i, vertex_i+1, vertex_i+2) ); //not needed
				vertex_i += 3;

				if (indexed_uvs.size() > 0)
				{
					uvs.push_back(indexed_uvs[unsigned int(v1.y) - 1]);
					uvs.push_back(indexed_uvs[unsigned int(v2.y) - 1]);
					uvs.push_back(indexed_uvs[unsigned int(v3.y) - 1]);
				}

				if (indexed_normals.size() > 0)
				{
					normals.push_back(indexed_normals[unsigned int(v1.z) - 1]);
					normals.push_back(indexed_normals[unsigned int(v2.z) - 1]);
					normals.push_back(indexed_normals[unsigned int(v3.z) - 1]);
				}
			}
		}
	}

	delete data;

	std::cout << "OBJ loaded." << std::endl;

	writeBin(binPath.c_str());

	loadCollisionModels();

	return true;
}

bool Mesh::loadBin(char const* filename) {
	if (FILE* binFile = fopen(filename, "rb")) {
		std::cout << "Loading Mesh from bin file..." << std::endl;

		int num_vertex, num_normals, num_uvs;

		fread(&num_vertex, sizeof(int), 1, binFile);
		fread(&num_normals, sizeof(int), 1, binFile);
		fread(&num_uvs, sizeof(int), 1, binFile);

		vertices.resize(num_vertex);
		normals.resize(num_normals);
		uvs.resize(num_uvs);

		fread(&vertices[0], sizeof(Vector3), num_vertex, binFile);
		fread(&normals[0], sizeof(Vector3), num_normals, binFile);
		fread(&uvs[0], sizeof(Vector2), num_uvs, binFile);
		fread(&center, sizeof(Vector3), 1, binFile);
		fread(&halfSize, sizeof(Vector3), 1, binFile);

		fclose(binFile);
		std::cout << "Bin file loaded." << std::endl;

		loadCollisionModels();

		return true;
	}
	return false;
}

std::vector<std::string> Mesh::tokenize(const std::string& source, const char* delimiters, bool process_strings)
{
	std::vector<std::string> tokens;

	std::string str;
	char del_size = strlen(delimiters);
	const char* pos = source.c_str();
	char in_string = 0;
	int i = 0;
	while (*pos != 0)
	{
		bool split = false;

		if (!process_strings || (process_strings && in_string == 0))
		{
			for (i = 0; i < del_size && *pos != delimiters[i]; i++);
			if (i != del_size) split = true;
		}

		if (process_strings && (*pos == '\"' || *pos == '\''))
		{
			if (!str.empty() && in_string == 0) //some chars remaining
			{
				tokens.push_back(str);
				str.clear();
			}

			in_string = (in_string != 0 ? 0 : *pos);
			if (in_string == 0)
			{
				str += *pos;
				split = true;
			}
		}

		if (split)
		{
			if (!str.empty())
			{
				tokens.push_back(str);
				str.clear();
			}
		}
		else
			str += *pos;
		pos++;
	}
	if (!str.empty())
		tokens.push_back(str);
	return tokens;
}


Vector3 Mesh::parseVector3(const char* text, const char separator)
{
	int pos = 0;
	char num[255];
	const char* start = text;
	const char* current = text;
	Vector3 result;

	while (1)
	{
		if (*current == separator || (*current == '\0' && current != text))
		{
			strncpy(num, start, current - start);
			num[current - start] = '\0';
			start = current + 1;
			if (num[0] != 'x') //¿?
				switch (pos)
				{
				case 0: result.x = (float)atof(num); break;
				case 1: result.y = (float)atof(num); break;
				case 2: result.z = (float)atof(num); break;
				default: return result; break;
				}

			++pos;
			if (*current == '\0')
				break;
		}

		++current;
	}

	return result;
};


void Mesh::writeBin(char const* bin_filename){
	if (FILE* binFile = fopen(bin_filename, "wb")) {
		int totalVertexs, totalNormals, totalUvVertexs;

		totalVertexs = vertices.size();
		totalNormals = normals.size();
		totalUvVertexs = uvs.size();

		fwrite(&totalVertexs, sizeof(int), 1, binFile);
		fwrite(&totalNormals, sizeof(int), 1, binFile);
		fwrite(&totalUvVertexs, sizeof(int), 1, binFile);

		fwrite(&vertices[0], sizeof(Vector3), totalVertexs, binFile);
		fwrite(&normals[0], sizeof(Vector3), totalVertexs, binFile);
		fwrite(&uvs[0], sizeof(Vector2), totalUvVertexs, binFile);

		//Coordinates for the bounding box
		Vector3 maxBounding = getMaxCoordinates(vertices);
		Vector3 minBounding = getMinCoordinates(vertices);

		center = (maxBounding + minBounding)*0.5;
		halfSize = maxBounding - center;

		fwrite(&center, sizeof(Vector3), 1, binFile);
		fwrite(&halfSize, sizeof(Vector3), 1, binFile);

		fclose(binFile);
	}
}

void Mesh::loadCollisionModels() {
	collisionModel = newCollisionModel3D();
	collisionModel->setTriangleNumber(vertices.size());
	for (int i = 0; i < vertices.size(); i += 3)
	{
		collisionModel->addTriangle(vertices[i].v, vertices[i + 1].v, vertices[i + 2].v);
	}
	collisionModel->finalize();

	collisionBox = newCollisionModel3D();
	collisionBox->setTriangleNumber(12);

	Vector3 corners[8] = {
		halfSize, Vector3(-halfSize.x, halfSize.y, halfSize.z),
		Vector3(halfSize.x, halfSize.y, -halfSize.z), Vector3(-halfSize.x, halfSize.y, -halfSize.z),
		Vector3(halfSize.x, -halfSize.y, halfSize.z), Vector3(-halfSize.x, -halfSize.y, halfSize.z),
		Vector3(halfSize.x, -halfSize.y, -halfSize.z), Vector3(-halfSize.x, -halfSize.y, -halfSize.z)
	};
	
	for (int i = 0; i < 8; i++)
		corners[i] = corners[i] + center;

	collisionBox->addTriangle(corners[0].v, corners[2].v, corners[3].v);
	collisionBox->addTriangle(corners[3].v, corners[1].v, corners[0].v);
	collisionBox->addTriangle(corners[0].v, corners[1].v, corners[5].v);
	collisionBox->addTriangle(corners[5].v, corners[4].v, corners[0].v);
	collisionBox->addTriangle(corners[0].v, corners[4].v, corners[6].v);
	collisionBox->addTriangle(corners[6].v, corners[2].v, corners[0].v);
	collisionBox->addTriangle(corners[2].v, corners[6].v, corners[7].v);
	collisionBox->addTriangle(corners[7].v, corners[3].v, corners[2].v);
	collisionBox->addTriangle(corners[3].v, corners[7].v, corners[5].v);
	collisionBox->addTriangle(corners[5].v, corners[1].v, corners[3].v);
	collisionBox->addTriangle(corners[6].v, corners[4].v, corners[5].v);
	collisionBox->addTriangle(corners[5].v, corners[7].v, corners[6].v);

	collisionBox->finalize();
}