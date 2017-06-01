#ifndef TEXTURE_MANAGER_CPP
#define TEXTURE_MANAGER_CPP

#include "TextureManager.h"
#include <iostream>

TextureManager* TextureManager::myManager = 0;

TextureManager::TextureManager() {
}

TextureManager::~TextureManager() {
}

Texture* TextureManager::getTexture(const char* filename) {
	
	if (!myManager)
		myManager = new TextureManager();

	auto search = myManager->textures.find(filename);

	if (search != myManager->textures.end()) {
		return search->second;
	}
	
	Texture* texture = NULL;
	texture = new Texture();
	if (!texture->load(filename) ) {
		std::cout << "Texture not found or error" << std::endl;
		exit(0);
	}

	myManager->textures[filename] = texture;
	return texture;
}

TextureManager* TextureManager::getInstance() {
	if (myManager == 0 )
		myManager = new TextureManager();

	return myManager;
}


#endif // !MESH_MANAGER_CPP
