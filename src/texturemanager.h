/*
	Manager de texturas.

	Dídac Serrano TJE 2017
*/


#ifndef TEXTURE_MANAGER_H 
#define TEXTURE_MANAGER_H

#include "texture.h"
#include <string>
#include <map>

class TextureManager{

public:
	static TextureManager* myManager;
	std::map< std::string, Texture* > textures;
	TextureManager();
	~TextureManager();
	static Texture* getTexture(const char* filename);
	static TextureManager* getInstance();

};


#endif
