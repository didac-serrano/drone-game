/*
	Clase para la coordinación entre las clases no jugables.

	Dídac Serrano TJE 2017
*/


#ifndef IA_MANAGER_H
#define	IA_MANAGER_H

#include "entity.h"
#include "utils.h"
#include "camera.h"
#include "IA.h"

/*
En el juego es importante la comunicacion entre las distintas entidades de IA.
Asi como distinguir las que se mueven de las que unicamente son detectores, etc.
Organizarse entre ellas e intercambiar estados e informacion.
Esta clase coordina las IAs.
Llegado el caso de tener tambien IAs favorables al jugador, seria necesario
crear dos IAs una para cada "equipo".
*/


//class IA_Locker;
//class IA_Detector;

class IA_Manager {
public:

	static IA_Manager* myIAManager;
	
	IA_Manager();
	~IA_Manager();

	Entity* playerEntity; //aka target
						  //SDL_Joystick* myJoystick;
						  //Camera* camera;

	//std::vector<IA_Locker*> lockerIAs;
	std::vector<IA_Turret*> staticIAs;
	std::vector<IA_Drone*> dynamicIAs;


	//void addLockerEntity(IA_Locker* newLocker);
	void addStaticEntity(IA_Turret* newTurret);
	void addDynamicEntity(IA_Drone* newDrone);
	void deleteStaticEntity(IA_Turret* turret);

	void update(double seconds_elapsed);
	void warnAll(Vector3 lastSeenEnemy);
	static IA_Manager* getInstance();

};

#endif // !IA_MANAGER_H
