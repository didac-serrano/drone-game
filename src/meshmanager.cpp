#ifndef MESH_MANAGER_CPP
#define MESH_MANAGER_CPP

#include "meshManager.h"
#include <iostream>

MeshManager* MeshManager::myManager = 0;

MeshManager::MeshManager() {}
MeshManager::~MeshManager() {}

Mesh* MeshManager::getMesh(const char* filename) {
	
	if (!myManager)
		myManager = new MeshManager();

	auto search = myManager->meshes.find(filename);

	if (search != myManager->meshes.end()) {
		return search->second;
	}
	
	Mesh* mesh = NULL;
	mesh = new Mesh();
	if (!mesh->loadMesh(filename) ) {
		std::cout << "Mesh " << filename << " not found or error" << std::endl;
		exit(0);
	}

	myManager->meshes[filename] = mesh;
	return mesh;
}

MeshManager* MeshManager::getInstance() {
	if (myManager == 0 )
		myManager = new MeshManager();

	return myManager;
}


#endif // !MESH_MANAGER_CPP
