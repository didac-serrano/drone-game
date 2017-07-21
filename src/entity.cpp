#ifndef ENTITY_CPP
#define ENTITY_CPP

#include "entity.h"
#include "meshmanager.h"
#include "texturemanager.h"
#include "bulletmanager.h"
#include "utils.h"

// Entity
unsigned Entity::idCounter = 0;
Shader* Entity::default_shader = NULL;

Entity::Entity()
{
	id = idCounter++;
	parent = NULL;
}
Entity::~Entity() {}


void Entity::render(Camera* camera) {
	for (int i = 0; i < children.size(); i++) {
		children[i]->render(camera);
	}
}
void Entity::update(float elapsed_time) {
	for (int i = 0; i < children.size(); i++) {
		children[i]->update(elapsed_time);
	}
}

void Entity::addChild(Entity* ent) {
	if (ent->parent)
	{
		std::cout << "Entity " << ent->name << " already has a parent";
		return;
	}
	ent->parent = this;
	children.push_back(ent);
}

void Entity::removeChild(Entity* ent) {
	auto it = std::find(children.begin(), children.end(), ent);
	if (it == children.end())
	{
		std::cout << "Entity " << ent->name << " is not a child of " << this->name << std::endl;
		return;
	}
	children.erase(it);
	ent->parent = NULL;
}

Matrix44 Entity::getGlobalMatrix()
{
	if (parent)
		return model * parent->getGlobalMatrix();
	return model;
}

Vector3 Entity::getPosition() {
	return getGlobalMatrix() * Vector3();
}


// EntityMesh
EntityMesh::EntityMesh()
{
	clip = true; //Passar per parametre, mes endavant
	shader = NULL;
	id = idCounter++;
}
EntityMesh::~EntityMesh() {}

void EntityMesh::render(Camera* camera)
{
	Mesh* mesh = MeshManager::getMesh(meshName.c_str());
	Texture* texture;

	Vector3 actualCenter = getGlobalMatrix() * mesh->center;

	if (!clip || camera->testSphereInFrustum(actualCenter, mesh->halfSize.length())) {

		if (!shader) shader = default_shader;

		shader->enable();
		shader->setMatrix44("u_model", model);
		Matrix44 mvp = model * camera->viewprojection_matrix;
		shader->setMatrix44("u_mvp", mvp);

		if (textureNames.size() == 1) {
			texture = TextureManager::getTexture((textureNames[0] + ".tga").c_str());
			shader->setTexture("u_texture", texture);
			mesh->render(GL_TRIANGLES, shader);
		}

		else {
			for (int i = 0; i < textureNames.size(); i++) {
				//std::cout << textureNames[i] << std::endl;
				texture = TextureManager::getTexture((textureNames[i] + ".tga").c_str());
				shader->setTexture(textureNames[i].c_str(), texture);
				mesh->render(GL_TRIANGLES, shader);
			}
		}

		shader->disable();

		#ifdef _DEBUG
			//Debugging purposes
			Mesh front;
			front.vertices.push_back(getPosition());
			Vector3 frontVector = getGlobalMatrix().frontVector() * float(30.0);
			Vector3 farPoint = frontVector + getPosition();
			front.vertices.push_back(farPoint);

			if (front.vertices.size() > 0) {
				glLineWidth(4);
				front.render(GL_LINES);
			}
			//-------
		#endif
		
		for (int i = 0; i < children.size(); i++) {
			children[i]->render(camera);
		}
	}
}


//EntityCollider
std::vector<EntityCollider*> EntityCollider::dynamicColliders;
std::vector<EntityCollider*> EntityCollider::staticColliders;

EntityCollider::EntityCollider() {}
EntityCollider::~EntityCollider() {}

void EntityCollider::onBulletCollision() {}

void EntityCollider::onStaticCollision(Vector3 collisionPoint) {}

void EntityCollider::onDynamicCollision(EntityCollider* colliderEntity) {}

void EntityCollider::setDynamic()
{
	dynamicColliders.push_back(this);
}

void EntityCollider::setStatic()
{
	staticColliders.push_back(this);
}


//EntityShooter
EntityShooter::EntityShooter() {};
EntityShooter::~EntityShooter() {};
void EntityShooter::render(Camera* camera)
{
	Mesh* mesh = MeshManager::getMesh(meshName.c_str());
	Texture* texture;

	Vector3 actualCenter = getGlobalMatrix()*mesh->center;

	if (!clip || camera->testSphereInFrustum(actualCenter, mesh->halfSize.length())) {

		if (!shader) shader = default_shader;

		if (hitCooldown > 0.0)
		{
			hit_shader->enable();
			hit_shader->setMatrix44("u_model", model);
			Matrix44 mvp = model * camera->viewprojection_matrix;
			hit_shader->setMatrix44("u_mvp", mvp);

			if (textureNames.size() == 1) {
				texture = TextureManager::getTexture((textureNames[0] + ".tga").c_str());
				hit_shader->setTexture("u_texture", texture);
				mesh->render(GL_TRIANGLES, shader);
			}

			else {
				for (int i = 0; i < textureNames.size(); i++) {
					//std::cout << textureNames[i] << std::endl;
					texture = TextureManager::getTexture((textureNames[i] + ".tga").c_str());
					hit_shader->setTexture(textureNames[i].c_str(), texture);
					mesh->render(GL_TRIANGLES, shader);
				}
			}

			shader->disable();
		}
		else
		{
			shader->enable();
			shader->setMatrix44("u_model", model);
			Matrix44 mvp = model * camera->viewprojection_matrix;
			shader->setMatrix44("u_mvp", mvp);

			if (textureNames.size() == 1) {
				texture = TextureManager::getTexture((textureNames[0] + ".tga").c_str());
				shader->setTexture("u_texture", texture);
				mesh->render(GL_TRIANGLES, shader);
			}

			else {
				for (int i = 0; i < textureNames.size(); i++) {
					//std::cout << textureNames[i] << std::endl;
					texture = TextureManager::getTexture((textureNames[i] + ".tga").c_str());
					shader->setTexture(textureNames[i].c_str(), texture);
					mesh->render(GL_TRIANGLES, shader);
				}
			}

			shader->disable();
		}

		#ifdef _DEBUG
			//Debugging purposes
			Mesh front;
			front.vertices.push_back(getPosition());
			Vector3 frontVector = getGlobalMatrix().frontVector() * float(30.0);
			Vector3 farPoint = frontVector + getPosition();
			front.vertices.push_back(farPoint);

			if (front.vertices.size() > 0) {
				glLineWidth(4);
				front.render(GL_LINES);
			}
			//-------
		#endif

		for (int i = 0; i < children.size(); i++) {
			children[i]->render(camera);
		}
	}
}


//Drone
Drone::Drone() {
	hitCooldown = 0.0;
	bulletCooldown = 0.6;
	bulletTime = 0.6;
	stunned = 0.0;
	dmgOnHit = 25;
	additionalPackets = 0;
	numBonus = 0;
	healthPoints = 100;
	win = FALSE;
}

Drone::Drone(float seconds, int hit) {
	hitCooldown = 0.0;
	bulletCooldown = seconds;
	bulletTime = seconds;
	stunned = 0.0;
	dmgOnHit = hit;
	additionalPackets = 0;
	numBonus = 0;
	win = FALSE;
}

Drone::~Drone() {};

void Drone::update(float dt) {
	bulletTime -= dt;
	hitCooldown -= dt;
	if (stunned > 0.0)
		stunned -= dt;
	if (lastCollision > 0.0)
		lastCollision -= dt;
}

void Drone::shoot() {
	if (bulletTime <= 0) {
		BulletManager::getInstance()->createBullet(getPosition(),
			getGlobalMatrix().frontVector() * 600, id, 4.8, Vector3(0.0, 0.0, 1.0));
		bulletTime = bulletCooldown;
	}
}

void Drone::onBulletCollision() {
	healthPoints -= dmgOnHit;
	hitCooldown = 0.1;
}

void Drone::onStaticCollision(Vector3 collisionPoint) {
	//std::cout << getPosition().x << " | " << getPosition().y << " | " << getPosition().z << std::endl;
	healthPoints -= 5;
	Vector3 dir = getPosition() - collisionPoint;
	dir.normalize();
	dir = dir * 32;
	model.traslateLocal(dir.x, dir.y, dir.z);
	//std::cout << dir.x << " | " << dir.y << " | " << dir.z << "\n" << std::endl;
	if (name != "DRONE2") {
		healthPoints -= 22;
	}
}

void Drone::onDynamicCollision(EntityCollider* colliderEntity) {
	//std::cout << getPosition().x << " | " << getPosition().y << " | " << getPosition().z << std::endl;
	healthPoints -= 3;
	Vector3 dir = getPosition() - colliderEntity->getPosition();
	dir.normalize();
	dir = dir * 32;
	model.traslateLocal(dir.x, dir.y, dir.z);
	//std::cout << dir.x << " | " << dir.y << " | " << dir.z << "\n" << std::endl;
	if (name != "DRONE2") {
		healthPoints -= 22;
	}
}

void Drone::onDeath() {
	std::cout << "lolo \n" << std::endl;
	auto it = std::find(dynamicColliders.begin(), dynamicColliders.end(), this);
	if (it == dynamicColliders.end())
	{
		std::cout << "Error removing dynamic collider " << this->name << std::endl;
		return;
	}
	dynamicColliders.erase(it);
	std::cout << "lolo \n" << std::endl;
	parent->removeChild(this);
	delete this;
}


//Turret
Turret::Turret() {
	hitCooldown = 0.0;
	bulletCooldown = 0.5;
	bulletTime = 0.5;
	healthPoints = 100;
	maxRange = 1570;
	minRange = 70;
};

Turret::Turret(float seconds) {
	bulletCooldown = seconds;
	bulletTime = seconds;
	healthPoints = 100;
	maxRange = 1570;
	minRange = 70;
};

Turret::~Turret() {};

void Turret::update(float dt) {
	bulletTime -= dt;
	hitCooldown -= dt;
}

void Turret::shoot() 
{
	Vector3 cannonShift = getPosition() + Vector3(0.0, 37.0, 0.0) + (getGlobalMatrix().frontVector() * 90);
	if (bulletTime <= 0) {
		BulletManager::getInstance()->createBullet(cannonShift,
			(targetPosition - cannonShift).normalize() * 500, id, 4.8, Vector3(1.0, 0.0, 0.0));
		bulletTime = bulletCooldown;
	}
}

void Turret::onBulletCollision() {
	healthPoints -= 25;
	hitCooldown = 0.1;
}

void Turret::onDeath() {
	auto it = std::find(dynamicColliders.begin(), dynamicColliders.end(), this);
	if (it == dynamicColliders.end())
	{
		std::cout << "Error removing dynamic collider " << this->name << std::endl;
		return;
	}
	dynamicColliders.erase(it);

	parent->removeChild(this);
	delete this;
}


//PowerUp
PowerUp::PowerUp(int tipus) {
	type = tipus;
}
PowerUp::~PowerUp() {};

void PowerUp::onDynamicCollision(EntityCollider* colliderEntity) {
	// només pot agafar-lo el player
	if (colliderEntity->id == playerEntity->id)
		onCollection();
}

void PowerUp::update(float seconds_elapsed) {
	model.rotateLocal(float(seconds_elapsed * 0.27), getGlobalMatrix().topVector());
}

void PowerUp::onCollection() {
	//+3 hp de la colisió
	//BLUE
	if (type == 1) {
		playerEntity->dmgOnHit -= 1;
		playerEntity->healthPoints += 3;
		playerEntity->numBonus += 1;
	}
	//GREEN
	else if (type == 2) {
		playerEntity->healthPoints += 53;
		playerEntity->numBonus += 1;
	}
	//RED
	else if (type == 3) {
		playerEntity->bulletTime -= 0.25;
		playerEntity->healthPoints += 3;
		playerEntity->numBonus += 1;
	}
	onDeath();
}

void PowerUp::onDeath() {
	auto it = std::find(dynamicColliders.begin(), dynamicColliders.end(), this);
	if (it == dynamicColliders.end())
	{
		std::cout << "Error removing dynamic collider " << this->name << std::endl;
		return;
	}
	dynamicColliders.erase(it);

	parent->removeChild(this);
	delete this;
}

//Packet
Packet::Packet(unsigned int tipus) {
	type = tipus;
	hitCooldown = 0.0;
}
Packet::~Packet() {};

void Packet::render(Camera* camera)
{
	Mesh* mesh = MeshManager::getMesh(meshName.c_str());
	Texture* texture;

	Vector3 actualCenter = getGlobalMatrix()*mesh->center;

	if (!clip || camera->testSphereInFrustum(actualCenter, mesh->halfSize.length())) {

		if (!shader) shader = default_shader;

		if (hitCooldown > 0.0)
		{
			hit_shader->enable();
			hit_shader->setMatrix44("u_model", model);
			Matrix44 mvp = model * camera->viewprojection_matrix;
			hit_shader->setMatrix44("u_mvp", mvp);

			if (textureNames.size() == 1) {
				texture = TextureManager::getTexture((textureNames[0] + ".tga").c_str());
				hit_shader->setTexture("u_texture", texture);
				mesh->render(GL_TRIANGLES, shader);
			}

			else {
				for (int i = 0; i < textureNames.size(); i++) {
					//std::cout << textureNames[i] << std::endl;
					texture = TextureManager::getTexture((textureNames[i] + ".tga").c_str());
					hit_shader->setTexture(textureNames[i].c_str(), texture);
					mesh->render(GL_TRIANGLES, shader);
				}
			}

			shader->disable();
		}
		else
		{
			shader->enable();
			shader->setMatrix44("u_model", model);
			Matrix44 mvp = model * camera->viewprojection_matrix;
			shader->setMatrix44("u_mvp", mvp);

			if (textureNames.size() == 1) {
				texture = TextureManager::getTexture((textureNames[0] + ".tga").c_str());
				shader->setTexture("u_texture", texture);
				mesh->render(GL_TRIANGLES, shader);
			}

			else {
				for (int i = 0; i < textureNames.size(); i++) {
					//std::cout << textureNames[i] << std::endl;
					texture = TextureManager::getTexture((textureNames[i] + ".tga").c_str());
					shader->setTexture(textureNames[i].c_str(), texture);
					mesh->render(GL_TRIANGLES, shader);
				}
			}

			shader->disable();
		}

#ifdef _DEBUG
		//Debugging purposes
		Mesh front;
		front.vertices.push_back(getPosition());
		Vector3 frontVector = getGlobalMatrix().frontVector() * float(30.0);
		Vector3 farPoint = frontVector + getPosition();
		front.vertices.push_back(farPoint);

		if (front.vertices.size() > 0) {
			glLineWidth(4);
			front.render(GL_LINES);
		}
		//-------
#endif

		for (int i = 0; i < children.size(); i++) {
			children[i]->render(camera);
		}
	}
}

void Packet::update(float seconds_elapsed) {
	model.rotateLocal(float(seconds_elapsed * 0.1), getGlobalMatrix().topVector());
	hitCooldown -= seconds_elapsed;
	if (!healthPoints) onDeath();
}

void Packet::onBulletCollision() {
	healthPoints -= 10;
	hitCooldown = 0.1;
}

void Packet::onDeath() {
	//Opcionals
	if (type == 1) {
		playerEntity->additionalPackets += 1;
		auto it = std::find(dynamicColliders.begin(), dynamicColliders.end(), this);
		if (it == dynamicColliders.end())
		{
			std::cout << "Error removing dynamic collider " << this->name << std::endl;
			return;
		}
		dynamicColliders.erase(it);

		parent->removeChild(this);
		delete this;
	}
	//Main
	else if (type == 2)
		playerEntity->win = TRUE;
}

#endif // !ENTITY_CPP
