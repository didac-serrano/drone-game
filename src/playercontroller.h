/*
	Controlador de los inputs del jugador.

	Dídac Serrano TJE 2017
*/

#ifndef PLAYERCONTROLLER_H
#define	PLAYERCONTROLLER_H

#include "entity.h"
#include "utils.h"
#include "camera.h"

class PlayerController {
public:
	Drone* playerEntity;
	SDL_Joystick* myJoystick;
	Camera* camera;

	PlayerController();
	~PlayerController();
	void update(double seconds_elapsed);

};

#endif // !PLAYERCONTROLLER_H
