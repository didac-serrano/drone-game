/*
	Clase para la gestion de entidades del juego,
	Establecer relaciones entre ellas y pintarlas en pantalla.

	Marc Aranda y Dídac Serrano TJE 2017
*/


#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include "framework.h"
#include "mesh.h"
#include "texture.h"
/*
	Clase para la gestión de todo elemento del juego que
	está sujeto al sistema de coordenadas, ya deba ser pintada o no.

	Dídac Serrano TJE 2017
*/


#include "shader.h"
#include "camera.h"
#include "extra/coldet/coldet.h"

class Entity
{
public:
	Entity(); //constructor 
	~Entity(); //destructor

	//some attributes 
	std::string name;
	Matrix44 model;
	static unsigned idCounter;
	unsigned id;

	//local vectors
	Vector3 front;
	
	static Shader* default_shader;
	Shader* hit_shader;
	Shader* shader;

	Entity* parent; //pointer to my parent entity 
 
    //pointers to my children
    std::vector<Entity*> children;
 
    //methods
    void addChild(Entity* ent); 
    void removeChild(Entity* ent);
 
	//methods overwriten by derived classes
	virtual void render(Camera* c);
	virtual void update(float elapsed_time);

	//some useful methods...
	Matrix44 getGlobalMatrix(); //returns transform in world coordinates
	Vector3 getPosition();
};


class EntityMesh : public Entity
{
public:
	//Attributes of this class 
	Vector3 color;
	std::string meshName;
	std::vector<std::string> textureNames;
	bool clip;

	EntityMesh();
	~EntityMesh();

	//methods overwriten 
	void render(Camera* camera);
};


class EntityCollider : public EntityMesh
{
public:
	static std::vector<EntityCollider*> dynamicColliders;
	static std::vector<EntityCollider*> staticColliders;
	float stunned;
	float lastCollision;

	EntityCollider();
	~EntityCollider();

	virtual void onBulletCollision();
	virtual void onStaticCollision(Vector3 collisionPoint);
	virtual void onDynamicCollision(EntityCollider* colliderEntity);
	void setDynamic();
	void setStatic();
};

class EntityShooter : public EntityCollider
{
public:
	float bulletTime;
	float bulletCooldown;
	float hitCooldown;
	int healthPoints;
	int maxRange;
	int minRange;
	//Se utilitza a IA pero es un atribut independent de cada entitat
	Vector3 targetPosition;

	EntityShooter();
	~EntityShooter();
	//virtual void shoot() = 0;

	//methods overwriten 
	void render(Camera* camera);
};

class Drone : public EntityShooter //Shooter
{
public:
	Drone();
	Drone(float seconds);
	~Drone();
	
	void update(float dt);
	void shoot();
	void onBulletCollision();
	void onStaticCollision(Vector3 collisionPoint);
	void onDynamicCollision(EntityCollider* colliderEntity);
	//Hereda de EntityCollider, perd healthPoints o el que sigui
};

class Turret : public EntityShooter //Shooter
{
public:
	Turret();
	Turret(float seconds);
	~Turret();

	void update(float dt);
	void shoot();
	void onBulletCollision();
	void onDynamicCollision(EntityCollider* colliderEntity);
	void onDeath();
	//Hereda de EntityCollider, perd healthPoints o el que sigui
};

#endif // !ENTITY_H
