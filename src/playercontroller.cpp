#include "playercontroller.h"
#include "game.h"
#include "bulletmanager.h"

PlayerController::PlayerController() {
	playerEntity = NULL;
	myJoystick = NULL;
	camera = NULL;
	inercia = {0, 0, 0};
}

PlayerController::~PlayerController() {}

void PlayerController::update(double seconds_elapsed) {
	
	double speed = seconds_elapsed * 350;
	double tiltAngle = 0.8;

	Matrix44 entityMatrix = playerEntity->getGlobalMatrix();
	Vector3 cameraMatrixEye = camera->eye + (entityMatrix * Vector3(0, 20, -60) - camera->eye) * float(speed * 0.03);
	Vector3 cameraMatrixCenter = camera->center + (entityMatrix * Vector3(0, 0, 100) - camera->center) * float(speed * 0.03);
	Vector3 cameraMatrixUp = camera->up + (entityMatrix.rotateVector(Vector3(0, 1, 0)) - camera->up) * float(speed * 0.03);
	camera->lookAt(cameraMatrixEye, cameraMatrixCenter, cameraMatrixUp);

	if (playerEntity->stunned > 0.0)
		return;

	//--------------------------//
	bool idle = TRUE;
	const Uint8* keystate = Game::keystate;

	//Y aka vertical
	if (keystate[SDL_SCANCODE_LSHIFT]) {
		//inercia = { 0, float(-speed * 0.4), 0 };
		playerEntity->model.traslateLocal(0, float(-speed * 0.4), 0);
		idle = FALSE;
	}
	if (keystate[SDL_SCANCODE_SPACE]) {
		//inercia = { 0, float(speed * 0.4), 0 };
		playerEntity->model.traslateLocal(0, float(speed * 0.4), 0);
		idle = FALSE;
	}
	//Z aka profunditat
	if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP]) {
		//inercia = { 0, 0, float(speed * 0.8) };
		playerEntity->model.traslateLocal(0, 0, float(speed * 0.8));
		idle = FALSE;
	}
	if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN]) {
		//inercia = { 0, 0, float(-speed * 0.5) };
		playerEntity->model.traslateLocal(0, 0, float(-speed * 0.5));
		idle = FALSE;
	}
	//X aka lateral
	if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT]) {
		//inercia = { float(speed * 0.45), 0, 0 };
		playerEntity->model.traslateLocal(float(speed * 0.45), 0, 0);
		idle = FALSE;
	}
	if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT]) {
		//inercia = { float(-speed * 0.45), 0, 0 };
		playerEntity->model.traslateLocal(float(-speed * 0.45), 0, 0);
		idle = FALSE;
	}
	if (keystate[SDL_SCANCODE_Q]) playerEntity->model.rotateLocal(float(-seconds_elapsed), Vector3(0, 1, 0)); //Eix Y (gir esquerre)
	if (keystate[SDL_SCANCODE_E]) playerEntity->model.rotateLocal(float(seconds_elapsed), Vector3(0, 1, 0)); //Eix Y (gir dreta)
	//if (idle) playerEntity->model.traslateLocal(inercia.x, inercia.y, inercia.z);
	if (keystate[SDL_SCANCODE_F]) playerEntity->shoot();
	
	if (playerEntity->healthPoints <= 0.0)
		*rip = TRUE;

	if (playerEntity->win)
		*win = TRUE;

	/*
	if (myJoystick) {

		JoystickState myJoystickState = getJoystickState(myJoystick);
		if (myJoystickState.axis[TRIGGERS_RT] > 0)
		{
			playerEntity->model.traslateLocal(0, 0, myJoystickState.axis[TRIGGERS_RT] * speed * 0.5);
		}
		if (abs(myJoystickState.axis[LEFT_ANALOG_X]) > 0.1)
		{
			playerEntity->model.rotateLocal(-myJoystickState.axis[LEFT_ANALOG_X] * 2 * seconds_elapsed, Vector3(0, 0, 1)); //Ok Eix Z (rota en rellotge) 
		}
		if (abs(myJoystickState.axis[LEFT_ANALOG_Y]) > 0.1)
		{
			playerEntity->model.rotateLocal(myJoystickState.axis[LEFT_ANALOG_Y] * seconds_elapsed, Vector3(1, 0, 0)); //Ok Eix X (puja i baixa)
		}
		if (myJoystickState.button[LEFT_ANALOG_BUTTON])
		{
			playerEntity->model.rotateLocal(-seconds_elapsed*0.5, Vector3(0, 1, 0)); //Eix Y (gir esquerre)
		}
		if (myJoystickState.button[RIGHT_ANALOG_BUTTON])
		{
			playerEntity->model.rotateLocal(seconds_elapsed*0.5, Vector3(0, 1, 0)); //Eix Y (gir dreta)
		}
		if (myJoystickState.button[A_BUTTON])
		{
			playerEntity->shoot();
		}
	}
	*/
}

void PlayerController::displayStatus() {

	std::string health = "Healtpoints: " + std::to_string(playerEntity->healthPoints);
	drawText(4, window_height - (window_height * 0.1), (health), Vector3(1, 1, 1), 2);
	float distance = mainPac->getPosition().distance(playerEntity->getPosition()) / 127;
	std::string quest = "[Destroy the main package] Distance: " + std::to_string(distance);
	drawText(4, window_height - (window_height * 0.075), (quest), Vector3(1, 1, 1), 2);
	std::string additional = "Optional packages destroyed: " + std::to_string(playerEntity->additionalPackets);
	drawText(4, window_height - (window_height * 0.05), (additional), Vector3(1, 1, 1), 2);
	std::string bonus = "Bonus collected: " + std::to_string(playerEntity->numBonus);
	drawText(4, window_height - (window_height * 0.025), (bonus), Vector3(1, 1, 1), 2);


}

void PlayerController::displayCoords() {

	std::string coordinates = std::to_string(playerEntity->getPosition().x) +
		" " + std::to_string(playerEntity->getPosition().y) +
		" " + std::to_string(playerEntity->getPosition().z);
	drawText(4, 22, (coordinates), Vector3(1, 1, 1), 2);
}

void PlayerController::render() {
	glDisable(GL_BLEND);
	displayStatus();
	//displayCoords();
	glEnable(GL_BLEND);
}