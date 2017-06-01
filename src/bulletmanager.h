/*
	Clases para la gestion de las balas en el juego.

	Dídac Serrano TJE 2017
*/


#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#define NUMBULLETS 50
#include "framework.h"
#include "camera.h"

class Bullet {
public:

	Vector3 position;
	Vector3 lastPosition;
	Vector3 vel;
	Vector3 bulletColor;

	float ttl;
	unsigned authorId;

	//unsigned type;

	Bullet();
	~Bullet();

};


class BulletManager {
public:

	static BulletManager* myBManager;

	Bullet bullets[NUMBULLETS];
	unsigned lastBulletPos;

	BulletManager();
	~BulletManager();

	void render(Camera* camera);
	void update(float elapsed_time);
	bool createBullet(Vector3 pos, Vector3 vel, unsigned id, float lifeTime, Vector3 color = Vector3(1.0, 1.0, 1.0));
	static BulletManager* getInstance();

};

#endif // !BULLETMANAGER_H
