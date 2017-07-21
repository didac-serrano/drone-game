/*
	Controlador de los inputs del jugador.
	Display por pantalla de sus atributos: health, boosts.

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
	Packet* mainPac;
	SDL_Joystick* myJoystick;
	Camera* camera;
	SDL_Window* window;
	Vector3 inercia;

	//display & gameplay
	int window_width;
	int window_height;
	bool* rip;
	bool* win;

	PlayerController();
	~PlayerController();
	void update(double seconds_elapsed);

	void render();
	void displayCoords();
	void displayStatus();
};

#endif // !PLAYERCONTROLLER_H
