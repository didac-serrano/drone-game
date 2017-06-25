#ifndef ENTITY_CPP
#define ENTITY_CPP

#include "extra/textparser.h"
#include "level.h"
#include "IA.h"

// Level
Level::Level() {}
Level::~Level() {}

bool Level::loadLevel(Entity* root, PlayerController* controller, IA_Manager* motherIA, char const* filename)
{
	std::string levelPath = "data/levels/";
	levelPath.append(filename);

	TextParser t;
	if (t.create(levelPath.c_str()) == false)return false;
	t.seek("*ENTITIES");
	int numEntities = t.getint();

	//load Player
	t.seek("*PLAYERENTITY");
	std::string playerString = t.getword();
	Drone* player = new Drone();
	player->healthPoints = 100;
	player->name = playerString;
	player->meshName = playerString;
	player->textureNames.push_back(playerString);
	
	t.seek("*SHADER");
	std::string shaderName = t.getword();

	if (shaderName != "DF") {
		std::string vsShaderPath = "data/shaders/" + shaderName + ".vs";
		std::string fsShaderPath = "data/shaders/" + shaderName + ".fs";
		player->shader = new Shader();
		if (!player->shader->load(vsShaderPath.c_str(), fsShaderPath.c_str()))
		{
			std::cout << "Shader not found or error" << std::endl;
			exit(0);
		}
	}

	/*
	t.seek("*SCALE");
	Vector3* playerScale = new Vector3;
	playerScale->x = t.getfloat();
	playerScale->y = t.getfloat();
	playerScale->z = t.getfloat();
	*/

	t.seek("*TRASLATE");
	Vector3* playerTranslate = new Vector3;
	playerTranslate->x = t.getfloat();
	playerTranslate->y = t.getfloat();
	playerTranslate->z = t.getfloat();
	
	Matrix44 modelPlayer;
	//modelPlayer.setScale(playerScale->x, playerScale->y, playerScale->z);
	modelPlayer.traslate(playerTranslate->x, playerTranslate->y, playerTranslate->z);
	player->model = modelPlayer;
	root->addChild(player);
	player->setDynamic();
	controller->playerEntity = player;
	motherIA->playerEntity = player;

	
	//load Turrets
	t.seek("*TURRETENTITIES");
	std::vector<Entity*> turretEntities;
	int numTurrets = t.getint();
	for (int i = 0; i < numTurrets; i++)
	{
		t.seek("*TURRET");
		std::string turretString = t.getword();
		Turret* turret = new Turret();
		turret->name = turretString;
		turret->meshName = turretString;
		
		/*
		t.seek("*N_TEXTURES");
		int numTextures = t.getint();

		for (int j = 0; j < numTextures; j++) {
			t.seek("*TEXTURE");
			std::string textureName = t.getword();
			turret->textureNames.push_back(textureName);
		}*/

		turret->textureNames.push_back(turretString);

		t.seek("*SHADER");
		std::string shaderName = t.getword();

		if (shaderName != "DF") { //ShaderManager
			std::string vsShaderPath = "data/shaders/" + shaderName + ".vs";
			std::string fsShaderPath = "data/shaders/" + shaderName + ".fs";
			turret->shader = new Shader();
			if (!turret->shader->load(vsShaderPath, fsShaderPath))
			{
				std::cout << "Shader not found or error" << std::endl;
				exit(0);
			}
		}

		t.seek("*TRASLATE");
		Vector3* mapTranslate = new Vector3;
		mapTranslate->x = t.getfloat();
		mapTranslate->y = t.getfloat();
		mapTranslate->z = t.getfloat();


		Matrix44 modelTurret;
		modelTurret.traslate(mapTranslate->x, mapTranslate->y, mapTranslate->z);
		turret->model = modelTurret;

		turretEntities.push_back(turret);
		turret->setDynamic();

		IA_Turret* turretIA = new IA_Turret();
		turretIA->controlledEntity = turret;
		turretIA->target = player;

		motherIA->addStaticEntity(turretIA);
	}
	
	for (int i = 0; i < numTurrets; i++)
	{
		root->addChild(turretEntities[i]);
	}



	//load Map Entities
	t.seek("*MAPBGENTITIES");
	int numMapEntities = t.getint();
	std::vector<Entity*> mapEntities;
	for (int i = 0; i < numMapEntities; i++)
	{
		t.seek("*MAPENTITY");
		EntityCollider* mapEntity = new EntityCollider();
		std::string mapString = t.getword();
		mapEntity->name = mapString;
		mapEntity->meshName = mapString;

		t.seek("*N_TEXTURES");
		int numTextures = t.getint();

		for (int j = 0; j < numTextures; j++) {
			t.seek("*TEXTURE");
			std::string textureName = t.getword();
			mapEntity->textureNames.push_back(textureName);
		}

		t.seek("*SHADER");
		std::string shaderName= t.getword();

		if (shaderName != "DF") { //ShaderManager
			std::string vsShaderPath = "data/shaders/" + shaderName + ".vs";
			std::string fsShaderPath = "data/shaders/" + shaderName + ".fs";
			mapEntity->shader = new Shader();
			if (!mapEntity->shader->load(vsShaderPath, fsShaderPath))
			{
				std::cout << "Shader not found or error" << std::endl;
				exit(0);
			}
		}

		/*
		t.seek("*SCALE");
		Vector3* mapScale = new Vector3;
		mapScale->x = t.getfloat();
		mapScale->y = t.getfloat();sss
		mapScale->z = t.getfloat();
		*/
		t.seek("*TRASLATE");
		Vector3* mapTranslate = new Vector3;
		mapTranslate->x = t.getfloat();
		mapTranslate->y = t.getfloat();
		mapTranslate->z = t.getfloat();


		Matrix44 modelMap;
		//modelMap.setScale(mapScale->x, mapScale->y, mapScale->z);
		//if (mapString == "GHOUSE")modelMap.setScale(10.0, 10.0, 10.0);
		modelMap.traslate(mapTranslate->x, mapTranslate->y, mapTranslate->z);
		mapEntity->model = modelMap;

		mapEntities.push_back(mapEntity);
		mapEntity->setStatic();
	}

	Entity* mapRoot = new Entity();
	for (int i = 0; i < numMapEntities; i++)
	{
		mapRoot->addChild(mapEntities[i]);
	}
	root->addChild(mapRoot);

	return true;
}

#endif