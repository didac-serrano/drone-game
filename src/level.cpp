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
	Drone* player = new Drone(0.6, 2);
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

	std::string vsShaderHitPath = "data/shaders/simple.vs";
	std::string fsShaderHitPath = "data/shaders/IMHIT.fs";
	player->hit_shader = new Shader();
	if (!player->hit_shader->load(vsShaderHitPath.c_str(), fsShaderHitPath.c_str()))
	{
		std::cout << "Hit Ssader not found or error" << std::endl;
		exit(0);
	}

	t.seek("*TRASLATE");
	Vector3* playerTranslate = new Vector3;
	playerTranslate->x = t.getfloat();
	playerTranslate->y = t.getfloat();
	playerTranslate->z = t.getfloat();
	
	Matrix44 modelPlayer;
	modelPlayer.traslate(playerTranslate->x, playerTranslate->y, playerTranslate->z);
	player->model = modelPlayer;
	root->addChild(player);
	player->setDynamic();
	controller->playerEntity = player;
	motherIA->playerEntity = player;

	// load Main pac
	t.seek("*MAINPKT");
	Packet* pac = new Packet(2);
	std::string packetString = t.getword();
	pac->name = packetString;
	pac->meshName = packetString;
	pac->playerEntity = player;
	pac->textureNames.push_back(packetString);
	pac->healthPoints = 300;

	t.seek("*SHADER");
	shaderName = t.getword();
	if (shaderName != "DF") { //ShaderManager
		std::string vsShaderPath = "data/shaders/" + shaderName + ".vs";
		std::string fsShaderPath = "data/shaders/" + shaderName + ".fs";
		pac->shader = new Shader();
		if (!pac->shader->load(vsShaderPath, fsShaderPath))
		{
			std::cout << "Shader not found or error" << std::endl;
			exit(0);
		}
	}

	pac->hit_shader = new Shader();
	if (!pac->hit_shader->load(vsShaderHitPath.c_str(), fsShaderHitPath.c_str()))
	{
		std::cout << "Hit shader not found or error" << std::endl;
		exit(0);
	}

	t.seek("*TRASLATE");
	Vector3* pacTranslate = new Vector3;
	pacTranslate->x = t.getfloat();
	pacTranslate->y = t.getfloat();
	pacTranslate->z = t.getfloat();

	Matrix44 modelPac;
	modelPac.traslate(pacTranslate->x, pacTranslate->y, pacTranslate->z);
	pac->model = modelPac;

	pac->setDynamic();
	root->addChild(pac);
	controller->mainPac = pac;
	

	//load Drones
	t.seek("*DRONEENTITIES");
	std::vector<Drone*> droneEntities;
	int numDrones = t.getint();
	for (int i = 0; i < numDrones; i++)
	{
		t.seek("*DRONE");
		std::string droneString = t.getword();
		Drone* drone = new Drone();
		drone->name = droneString;
		drone->meshName = droneString;

		drone->textureNames.push_back(droneString);

		t.seek("*SHADER");
		std::string shaderName = t.getword();

		if (shaderName != "DF") { //ShaderManager
			std::string vsShaderPath = "data/shaders/" + shaderName + ".vs";
			std::string fsShaderPath = "data/shaders/" + shaderName + ".fs";
			drone->shader = new Shader();
			if (!drone->shader->load(vsShaderPath, fsShaderPath))
			{
				std::cout << "Shader not found or error" << std::endl;
				exit(0);
			}
		}

		std::string vsShaderHitPath = "data/shaders/simple.vs";
		std::string fsShaderHitPath = "data/shaders/IMHIT.fs";
		drone->hit_shader = new Shader();
		if (!drone->hit_shader->load(vsShaderHitPath.c_str(), fsShaderHitPath.c_str()))
		{
			std::cout << "Hit shader not found or error" << std::endl;
			exit(0);
		}

		t.seek("*TRASLATE");
		Vector3* mapTranslate = new Vector3;
		mapTranslate->x = t.getfloat();
		mapTranslate->y = t.getfloat();
		mapTranslate->z = t.getfloat();


		Matrix44 modelDrone;
		modelDrone.traslate(mapTranslate->x, mapTranslate->y, mapTranslate->z);
		drone->model = modelDrone;

		droneEntities.push_back(drone);
		drone->setDynamic();

		IA_Drone* droneIA = new IA_Drone();
		droneIA->controlledEntity = drone;
		droneIA->target = player;

		motherIA->addDynamicEntity(droneIA);
	}

	for (int i = 0; i < numDrones; i++)
	{
		root->addChild(droneEntities[i]);
	}

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

		std::string vsShaderHitPath = "data/shaders/simple.vs";
		std::string fsShaderHitPath = "data/shaders/IMHIT.fs";
		turret->hit_shader = new Shader();
		if (!turret->hit_shader->load(vsShaderHitPath.c_str(), fsShaderHitPath.c_str()))
		{
			std::cout << "Hit shader not found or error" << std::endl;
			exit(0);
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


	//load RapidTurret
	t.seek("*RAPID_TURRETS");
	std::vector<Entity*> rapidEntities;
	int numRapid = t.getint();
	for (int i = 0; i < numRapid; i++)
	{
		t.seek("*RAPID_TURRET");
		std::string turretString = t.getword();
		Turret* turret = new Turret(0.09);
		turret->name = turretString;
		turret->meshName = turretString;

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

		std::string vsShaderHitPath = "data/shaders/simple.vs";
		std::string fsShaderHitPath = "data/shaders/IMHIT.fs";
		turret->hit_shader = new Shader();
		if (!turret->hit_shader->load(vsShaderHitPath.c_str(), fsShaderHitPath.c_str()))
		{
			std::cout << "Hit shader not found or error" << std::endl;
			exit(0);
		}

		t.seek("*TRASLATE");
		Vector3* mapTranslate = new Vector3;
		mapTranslate->x = t.getfloat();
		mapTranslate->y = t.getfloat();
		mapTranslate->z = t.getfloat();


		Matrix44 modelTurret;
		modelTurret.traslate(mapTranslate->x, mapTranslate->y, mapTranslate->z);
		turret->model = modelTurret;

		rapidEntities.push_back(turret);
		turret->setDynamic();

		IA_Turret* turretIA = new IA_Turret();
		turretIA->controlledEntity = turret;
		turretIA->target = player;

		motherIA->addStaticEntity(turretIA);
	}

	for (int i = 0; i < numRapid; i++)
	{
		root->addChild(rapidEntities[i]);
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

		t.seek("*TRASLATE");
		Vector3* mapTranslate = new Vector3;
		mapTranslate->x = t.getfloat();
		mapTranslate->y = t.getfloat();
		mapTranslate->z = t.getfloat();

		Matrix44 modelMap;
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


	//load PowerUPs
	std::vector<Entity*> powerUps;
	t.seek("*POWERUP_BLUE");
	int numBluePow = t.getint();
	for (int i = 0; i < numBluePow; i++)
	{
		PowerUp* powerUp = new PowerUp(1);
		std::string powerString = t.getword();
		powerUp->name = powerString;
		powerUp->meshName = "powerup";
		powerUp->playerEntity = player;
		powerUp->textureNames.push_back(powerString);

		t.seek("*SHADER");
		std::string shaderName = t.getword();
		if (shaderName != "DF") { //ShaderManager
			std::string vsShaderPath = "data/shaders/" + shaderName + ".vs";
			std::string fsShaderPath = "data/shaders/" + shaderName + ".fs";
			powerUp->shader = new Shader();
			if (!powerUp->shader->load(vsShaderPath, fsShaderPath))
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

		Matrix44 modelMap;
		modelMap.traslate(mapTranslate->x, mapTranslate->y, mapTranslate->z);
		powerUp->model = modelMap;

		powerUps.push_back(powerUp);
		powerUp->setDynamic();
	}

	t.seek("*POWERUP_GREEN");
	int numGreenPow = t.getint();
	for (int i = 0; i < numGreenPow; i++)
	{
		PowerUp* powerUp = new PowerUp(2);
		std::string powerString = t.getword();
		powerUp->name = powerString;
		powerUp->meshName = "powerup";
		powerUp->playerEntity = player;
		powerUp->textureNames.push_back(powerString);

		t.seek("*SHADER");
		std::string shaderName = t.getword();
		if (shaderName != "DF") { //ShaderManager
			std::string vsShaderPath = "data/shaders/" + shaderName + ".vs";
			std::string fsShaderPath = "data/shaders/" + shaderName + ".fs";
			powerUp->shader = new Shader();
			if (!powerUp->shader->load(vsShaderPath, fsShaderPath))
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

		Matrix44 modelMap;
		modelMap.traslate(mapTranslate->x, mapTranslate->y, mapTranslate->z);
		powerUp->model = modelMap;

		powerUps.push_back(powerUp);
		powerUp->setDynamic();
	}

	t.seek("*POWERUP_RED");
	int numRedPow = t.getint();
	for (int i = 0; i < numRedPow; i++)
	{
		PowerUp* powerUp = new PowerUp(3);
		std::string powerString = t.getword();
		powerUp->name = powerString;
		powerUp->meshName = "powerup";
		powerUp->playerEntity = player;
		powerUp->textureNames.push_back(powerString);

		t.seek("*SHADER");
		std::string shaderName = t.getword();
		if (shaderName != "DF") { //ShaderManager
			std::string vsShaderPath = "data/shaders/" + shaderName + ".vs";
			std::string fsShaderPath = "data/shaders/" + shaderName + ".fs";
			powerUp->shader = new Shader();
			if (!powerUp->shader->load(vsShaderPath, fsShaderPath))
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

		Matrix44 modelMap;
		modelMap.traslate(mapTranslate->x, mapTranslate->y, mapTranslate->z);
		powerUp->model = modelMap;

		powerUps.push_back(powerUp);
		powerUp->setDynamic();
	}

	for (int i = 0; i < (numBluePow + numGreenPow + numRedPow); i++)
	{
		root->addChild(powerUps[i]);
	}


	// load Packets
	std::vector<Entity*> packets;
	t.seek("*PACKETS");
	int numPackets = t.getint();
	for (int i = 0; i < numPackets; i++)
	{
		t.seek("*PACKET");
		Packet* packet = new Packet(1);
		std::string packetString = t.getword();
		packet->name = packetString;
		packet->meshName = packetString;
		packet->playerEntity = player;
		packet->textureNames.push_back(packetString);
		packet->healthPoints = 100;

		t.seek("*SHADER");
		std::string shaderName = t.getword();
		if (shaderName != "DF") { //ShaderManager
			std::string vsShaderPath = "data/shaders/" + shaderName + ".vs";
			std::string fsShaderPath = "data/shaders/" + shaderName + ".fs";
			packet->shader = new Shader();
			if (!packet->shader->load(vsShaderPath, fsShaderPath))
			{
				std::cout << "Shader not found or error" << std::endl;
				exit(0);
			}
		}

		packet->hit_shader = new Shader();
		if (!packet->hit_shader->load(vsShaderHitPath.c_str(), fsShaderHitPath.c_str()))
		{
			std::cout << "Hit shader not found or error" << std::endl;
			exit(0);
		}

		t.seek("*TRASLATE");
		Vector3* mapTranslate = new Vector3;
		mapTranslate->x = t.getfloat();
		mapTranslate->y = t.getfloat();
		mapTranslate->z = t.getfloat();

		Matrix44 modelMap;
		modelMap.traslate(mapTranslate->x, mapTranslate->y, mapTranslate->z);
		packet->model = modelMap;

		packets.push_back(packet);
		packet->setDynamic();
	}

	for (int i = 0; i < numPackets; i++)
	{
		root->addChild(packets[i]);
	}

	return true;
}

#endif