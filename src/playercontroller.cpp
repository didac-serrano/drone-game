#include "playercontroller.h"
#include "game.h"
#include "bulletmanager.h"

PlayerController::PlayerController() {
	playerEntity = NULL;
	myJoystick = NULL;
	camera = NULL;
}

PlayerController::~PlayerController() {}

void PlayerController::update(double seconds_elapsed) {
	int speed = seconds_elapsed * 200;
	double tiltAngle = 0.8;

	//playerEntity->model.traslateLocal(0, 0, speed);

	const Uint8* keystate = Game::keystate;

	if (keystate[SDL_SCANCODE_LSHIFT]) playerEntity->model.traslateLocal(0, -2, 0);
	if (keystate[SDL_SCANCODE_SPACE]) playerEntity->model.traslateLocal(0, 2, 0);
	if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP])playerEntity->model.traslateLocal(0, 0, 5);
	if (keystate[SDL_SCANCODE_S] || keystate[SDL_SCANCODE_DOWN])playerEntity->model.traslateLocal(0, 0, -5);
	if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT])playerEntity->model.traslateLocal(5, 0, 0);
	if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT])playerEntity->model.traslateLocal(-5, 0, 0);
	if (keystate[SDL_SCANCODE_Q]) playerEntity->model.rotateLocal(-seconds_elapsed, Vector3(0, 1, 0)); //Eix Y (gir esquerre)
	if (keystate[SDL_SCANCODE_E]) playerEntity->model.rotateLocal(seconds_elapsed, Vector3(0, 1, 0)); //Eix Y (gir dreta)
	if (keystate[SDL_SCANCODE_F]) playerEntity->shoot();

	if (myJoystick) {

		JoystickState myJoystickState = getJoystickState(myJoystick);
		if (myJoystickState.axis[TRIGGERS_RT] > 0)
			playerEntity->model.traslateLocal(0, 0, myJoystickState.axis[TRIGGERS_RT] * speed * 0.5);

		if (abs(myJoystickState.axis[LEFT_ANALOG_X]) > 0.1)
			playerEntity->model.rotateLocal(-myJoystickState.axis[LEFT_ANALOG_X] * 2 * seconds_elapsed, Vector3(0, 0, 1)); //Ok Eix Z (rota en rellotge) 
		
		if (abs(myJoystickState.axis[LEFT_ANALOG_Y]) > 0.1)
			playerEntity->model.rotateLocal(myJoystickState.axis[LEFT_ANALOG_Y] * seconds_elapsed, Vector3(1, 0, 0)); //Ok Eix X (puja i baixa)
		
		if (myJoystickState.button[LEFT_ANALOG_BUTTON])
			playerEntity->model.rotateLocal(-seconds_elapsed*0.5, Vector3(0, 1, 0)); //Eix Y (gir esquerre)
		
		if (myJoystickState.button[RIGHT_ANALOG_BUTTON])
			playerEntity->model.rotateLocal(seconds_elapsed*0.5, Vector3(0, 1, 0)); //Eix Y (gir dreta)

		if (myJoystickState.button[A_BUTTON])
			playerEntity->shoot();

	}
	
	float factor = 0.5;
	Matrix44 entityMatrix = playerEntity->getGlobalMatrix();

	Vector3 cameraMatrixEye = entityMatrix * Vector3(0, 20, -60); //p38
	cameraMatrixEye = mix( cameraMatrixEye, camera->eye, factor);
	Vector3 cameraMatrixCenter = entityMatrix * Vector3(0, 0, 100);
	cameraMatrixCenter = mix(cameraMatrixCenter, camera->center, factor);
	//Vector3 cameraMatrixUp = entityMatrix.rotateVector(Vector3(0, 1, 0));
	//cameraMatrixUp = mix(cameraMatrixUp, camera->up, factor);
	
	camera->lookAt(cameraMatrixEye, cameraMatrixCenter, camera->up);

	
}