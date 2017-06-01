#ifndef IA_CPP
#define IA_CPP

#include "playercontroller.h"
#include "game.h"
#include "bulletmanager.h"
#include "IA.h"
#include "checkcollisions.h"
#include "framework.h"


//IA
IA::IA() {
	lastAngle = 0.0;
}
IA::~IA() {}


// IA_Turret
IA_Turret::IA_Turret() {};
IA_Turret::~IA_Turret() {};

void IA_Turret::update(double seconds_elapsed)
{
	controlledEntity->targetPosition = target->getPosition();
	
	if (scanEnemies())
	{
		rotate(seconds_elapsed);
		shootAtEnemies();
	}
}

bool IA_Turret::scanEnemies() 
{
	if (target->getPosition().distance((controlledEntity->getPosition())) < controlledEntity->detectionRange)
		return TRUE;
	return FALSE;
}

void IA_Turret::rotate(double seconds_elapsed)
{
	Vector3 direction = (target->getGlobalPosition() - controlledEntity->getGlobalPosition()).normalize();
	Vector3 entityFront = controlledEntity->getFront().normalize();

	//Necessitem que no tingui en compte la diferencia d'alçada, ja que només rota en un pla
	//Per això passem de les Y, sinó rota encara que estigui alineada amb el target (però en diferent alçada)
	double dot = (direction.x * entityFront.x) + (direction.z * entityFront.z);
	double cross = (direction.x * entityFront.z) - (direction.z * entityFront.x);
	double angle = acos(dot);

	//Evitem soroll en el calcul d'angles
	//Aka sense moure el target es movia la torreta
	double angleDif = abs(angle - lastAngle);

	//Es torreta, rota sobre el seu eix Y ja que està ancorada a una base
	if (angleDif > 0.01)
	{
		if (cross > 0.2) {
			controlledEntity->model.rotateLocal(-seconds_elapsed * angle * 2, controlledEntity->getTop());
		}
		else if (cross < 0.2) {
			controlledEntity->model.rotateLocal(seconds_elapsed * angle * 2, controlledEntity->getTop());
		}
	}
	lastAngle = angle;
}

void IA_Turret::shootAtEnemies()
{
	controlledEntity->shoot();
}

void IA_Turret::warn()
{
	//Aquesta funcio l'hem de pensar be, potser seria bo fer-ho desde fora,
	//tenir una funcio que recorre tots els Detectors que tenim i si algun ha detectat
	// el player posem a true el targetDetected de tots els controllers, i alhora a l'update
	// tenim que si targetDetected es true es dirigiran cap alla (per exemple)
	//motherIA->warnAll(lastSeenEnemy);
}

void IA_Turret::makeSound()
{
	std::cout << "Piiiii" << std::endl;
}









//IA_Drone
IA_Drone::IA_Drone() {}
IA_Drone::~IA_Drone() {}

void IA_Drone::update(double seconds_elapsed) {
	//Matrix44 myMatrix = controlledEntity->getGlobalMatrix();
	//Mensaje de Agenjo: "Primero haced que esta entidad este mirando siempre hacia player,
	// continuad a partir de ahi."

}



//IA_Detector
/*
IA_Detector::IA_Detector() {};

IA_Detector::~IA_Detector() {};

void IA_Detector::update(double seconds_elapsed)
{
	scanEnemies();
	makeSound();
}

bool IA_Detector::scanEnemies()
{
	//Potser caldria implementar aquesta funcio dins de IA directament, ja que també la utilitzaran
	// drone i turret

	/*
	Vector3 playerPos = motherIA->playerEntity->getPosition();
	Vector3 iaPos = getPosition();
	
	if (playerPos.distance(iaPos) > detectionRange)
	{
		return FALSE;
	}

	Vector3 ray = playerPos - iaPos; 
	//vector amb direccio i longitud adecuades
	//entre la entityIA i el player. Si al llarg del vector no hi ha cap colisio el veu
	//excepte si colisiona amb player, llavors tambe el veu

	//falta trobar la manera de comprovar que no hi ha cap entity enmig
	//suposo que haurem de fer-ho semblant al collision pero no me nhe sortit

	lastSeenEnemy = playerPos;
	warn();
	return TRUE;

}

void IA_Detector::warn()
{
	//Aquesta funcio l'hem de pensar be, potser seria bo fer-ho desde fora,
	//tenir una funcio que recorre tots els Detectors que tenim i si algun ha detectat
	// el player posem a true el targetDetected de tots els controllers, i alhora a l'update
	// tenim que si targetDetected es true es dirigiran cap alla (per exemple)
	//motherIA->warnAll(lastSeenEnemy);
}

void IA_Detector::makeSound()
{
	std::cout << "Piiiii" << std::endl;
} */


#endif // !IA_CPP