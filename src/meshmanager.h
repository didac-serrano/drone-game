/*
	Singleton para el manager de meshes.

	Dídac Serrano TJE 2017
*/


#ifndef MESH_MANAGER_H 
#define MESH_MANAGER_H

#include "mesh.h"
#include <string>
#include <map>

class MeshManager {

public:
	static MeshManager* myManager;
	std::map< std::string, Mesh* > meshes;
	MeshManager();
	~MeshManager();
	static Mesh* getMesh(const char* filename);
	static MeshManager* getInstance();
};


#endif
