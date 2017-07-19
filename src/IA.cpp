#ifndef IA_CPP
#define IA_CPP

#include "playercontroller.h"
#include "game.h"
#include "bulletmanager.h"
#include "IA.h"
#include "checkcollisions.h"
#include "framework.h"

//IA
IA::IA() {}
IA::~IA() {}


// IA_Turret
IA_Turret::IA_Turret() {
	angle = 0.0;
	angleDif = 0.0;
};
IA_Turret::~IA_Turret() {};

void IA_Turret::update(double seconds_elapsed)
{
	controlledEntity->targetPosition = target->getPosition();
	
	if (scanEnemies())
	{
		rotate(seconds_elapsed);
		shootAtEnemies();
	}
	if (controlledEntity->healthPoints <= 0.0) {
		//parent de turret aka controlledEntity es root
		controlledEntity->onDeath();
		IA_Manager::getInstance()->deleteStaticEntity(this);
		delete this;
	}
}

bool IA_Turret::scanEnemies() 
{
	if (target->getPosition().distance((controlledEntity->getPosition())) < controlledEntity->maxRange && 
		target->getPosition().distance((controlledEntity->getPosition())) > controlledEntity->minRange)
		return TRUE;
	return FALSE;
}

void IA_Turret::rotate(double seconds_elapsed)
{
	Vector3 direction = (target->getPosition() - controlledEntity->getPosition()).normalize();
	Vector3 entityFront = controlledEntity->getGlobalMatrix().frontVector().normalize();
	Vector3 entityRight = controlledEntity->getGlobalMatrix().rightVector().normalize();

	//Necessitem que no tingui en compte la diferencia d'alçada, ja que només rota en un pla
	//Per això passem de les Y, sinó rota encara que estigui alineada amb el target (però en diferent alçada)
	double dot = (direction.x * entityFront.x) + (direction.z * entityFront.z);
	double dot_frontUp = (direction.x * entityRight.x) + (direction.z * entityRight.z);
	angle = acos(dot);

	//Evitem soroll en el calcul d'angles
	//Aka sense moure el target es movia la torreta
	//std::cout << angle << "\n";
	//std::cout << dot_frontUp << "\n";
	//std::cout << ".........." << "\n";

	//Es torreta, rota sobre el seu eix Y ja que està ancorada a una base
	if (angle > 0.05)
	{
		if (dot_frontUp > 0.0) {
			controlledEntity->model.rotateLocal(float(-seconds_elapsed * angle), controlledEntity->getGlobalMatrix().topVector());
		}
		else if (dot_frontUp < 0.0) {
			controlledEntity->model.rotateLocal(float(seconds_elapsed * angle), controlledEntity->getGlobalMatrix().topVector());
		}
	}
}

void IA_Turret::shootAtEnemies()
{
	if(angle < 0.1)
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


#endif // !IA_CPP