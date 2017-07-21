/*
	Clases para la gestion del comportamiento de las entidades del juego
	no jugables.

	Dídac Serrano TJE 2017
*/


#ifndef IA_H
#define	IA_H

#include "entity.h"
#include "utils.h"
#include "camera.h"
#include "IA_manager.h"

/*
	En el juego es importante la comunicacion entre las distintas entidades de IA.
	Asi como distinguir las que se mueven de las que unicamente son detectores, etc.
	Organizarse entre ellas e intercambiar estados e informacion.
	Esta clase coordina las IAs.
	Llegado el caso de tener tambien IAs favorables al jugador, seria necesario
	crear dos IAs una para cada "equipo".
*/


//Clase para las caracteristicas generales que toda IA del juego debe tener
class IA {
public:

	IA();
	~IA();

	double angle;
	double angleDif;

	Entity* target;
	bool targetDetected;
	virtual void update(double seconds_elapsed) = 0;

};

/*
Torreta que busca enemigos y si los ve avisa a los otros bots y además les dispara.
Por defecto, su rango de deteccion es menor que el del detecor
*/
class IA_Turret : public IA {
public:
	IA_Turret();
	~IA_Turret();

	Turret* controlledEntity;
	void update(double seconds_elapsed);

	bool scanEnemies();
	void rotate(double seconds_elapsed);
	void shootAtEnemies();

	void warn();
	void makeSound(); //Ja se que de moment no hem implementat so (just the dream)
};

// DYNAMIC IAs (en nuestro caso los drones enemigos, se desplazan por el mapa)
class IA_Drone : public IA {
public:

	IA_Drone();
	~IA_Drone();

	Drone* controlledEntity;
	void update(double seconds_elapsed);
	
	bool scanEnemies();
	void rotate(double seconds_elapsed);
	void altitude(double seconds_elapsed);
	void translate(double seconds_elapsed);


};

#endif // !IA_H
