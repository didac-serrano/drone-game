/*
	Clase para la carga de niveles del juego,
	Establecer entidades y su posiciones.

	Dídac Serrano TJE 2017
*/

#ifndef LEVEL_H
#define LEVEL_H

#include "entity.h"
#include "playercontroller.h"
#include "IA_manager.h"

class Level
{
public:
	Level();
	~Level();

	bool loadLevel(Entity* root, PlayerController* controller, IA_Manager* motherIA, char const* filename);

};

#endif