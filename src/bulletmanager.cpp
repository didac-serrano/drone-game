#include "bulletmanager.h"
#include "mesh.h"
#include "texture.h"

Bullet::Bullet() {}
Bullet::~Bullet() {}

BulletManager* BulletManager::myBManager = 0;

BulletManager::BulletManager() {
	if (myBManager == 0) {
		myBManager = this;
		myBManager->lastBulletPos = 0;

		for (int i = 0; i < NUMBULLETS; i++) {
			Bullet* bullet = &myBManager->bullets[i];
			bullet->ttl = 0;
			bullet->bulletColor = Vector3(1.0, 1.0, 1.0);
		}
	}
}

BulletManager::~BulletManager() {}

void BulletManager::render(Camera* camera) {
	Mesh bulletMesh;

	for (int i = 0; i < NUMBULLETS; i++) {
		Bullet& bullet = myBManager->bullets[i];
		if (bullet.ttl > 0.0) {
			bulletMesh.vertices.push_back(bullet.lastPosition);
			bulletMesh.vertices.push_back(bullet.position);
			bulletMesh.colors.push_back(Vector4(bullet.bulletColor, 0));
			bulletMesh.colors.push_back(Vector4(bullet.bulletColor, 0));
		}
	}

	if (bulletMesh.vertices.size() > 0) {
		glLineWidth(2);
		glEnable(GL_BLEND);
		glLineWidth(4);
		bulletMesh.render(GL_LINES);
		glDisable(GL_BLEND);
	}
}

void BulletManager::update(float elapsed_time) {

	for (int i = 0; i < NUMBULLETS; i++) {
		Bullet& bullet = myBManager->bullets[i];
		if (bullet.ttl > 0.0) {
			bullet.lastPosition = bullet.position;
			bullet.position = bullet.position + bullet.vel * elapsed_time;
			bullet.ttl -= elapsed_time;
		}
	}
}

bool BulletManager::createBullet(Vector3 pos, Vector3 vel, unsigned id, float lifeTime, Vector3 color ) {
	
	for (int i = myBManager->lastBulletPos; i < NUMBULLETS; i++) {
		Bullet& bullet = myBManager->bullets[i];

		if (bullet.ttl <= 0.0 || &bullet == NULL) {
			bullet.position = pos;
			bullet.lastPosition = pos;
			bullet.vel = vel;
			bullet.authorId = id;
			bullet.ttl = lifeTime;
			bullet.bulletColor = color;
			myBManager->lastBulletPos = i;
			return true;
		}
	}


	myBManager->lastBulletPos = 0;
	return false;
}

BulletManager* BulletManager::getInstance() {
	if (myBManager == 0)
		new BulletManager();
		
	return myBManager;
}