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

	Vector3 actualCenter = getGlobalMatrix()*mesh->center;

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

		//Debugging purposes
		Mesh front;
		front.vertices.push_back(getPosition());
		Vector3 frontVector = getGlobalMatrix().frontVector() * float(30.0);
		Vector3 farPoint = frontVector + getPosition();
		front.vertices.push_back(farPoint);
		//std::cout << frontVector.x << " " << frontVector.y << " " << frontVector.z << "\n";
		if (front.vertices.size() > 0) {
			glLineWidth(4);
			front.render(GL_LINES);
		}
		//-------

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

		//Debugging purposes
		Mesh front;
		front.vertices.push_back(getPosition());
		Vector3 frontVector = getGlobalMatrix().frontVector() * float(30.0);
		Vector3 farPoint = frontVector + getPosition();
		front.vertices.push_back(farPoint);
		//std::cout << frontVector.x << " " << frontVector.y << " " << frontVector.z << "\n";
		if (front.vertices.size() > 0) {
			glLineWidth(4);
			front.render(GL_LINES);
		}
		//-------

		for (int i = 0; i < children.size(); i++) {
			children[i]->render(camera);
		}
	}
}


//Drone
Drone::Drone() {
	hitCooldown = 0.0;
	bulletCooldown = 2.0;
	bulletTime = 2.0;
	stunned = 0.0;
}

Drone::Drone(float seconds) {
	bulletCooldown = seconds;
	bulletTime = seconds;
	stunned = 0.0;
}

Drone::~Drone() {};

void Drone::update(float dt) {
	bulletTime -= dt;
	hitCooldown -= dt;
	if (stunned > 0.0)
		stunned -= dt;
	if (lastCollision > 0.0)
		lastCollision -= dt;
	if (healthPoints <= 0.0) {
		//endgame
	}
}

void Drone::shoot() {
	if (bulletTime <= 0) {
		BulletManager::getInstance()->createBullet(getPosition(),
			getGlobalMatrix().frontVector() * 600, id, 5.8, Vector3(0.0, 0.0, 1.0));
		bulletTime = bulletCooldown;
	}
}

void Drone::onBulletCollision() {
	healthPoints -= 5;
	hitCooldown = 0.1;
}

void Drone::onStaticCollision(Vector3 collisionPoint) {
	std::cout << getPosition().x << " | " << getPosition().y << " | " << getPosition().z << std::endl;
	healthPoints -= 5;
	Vector3 dir = getPosition() - collisionPoint;
	dir.normalize();
	dir = dir * 32;
	model.traslateLocal(dir.x, dir.y, dir.z);
	std::cout << dir.x << " | " << dir.y << " | " << dir.z << "\n" << std::endl;
}

void Drone::onDynamicCollision(EntityCollider* colliderEntity) {
	//agenjo "buscar física de particulas básica"
	healthPoints -= 1;
	/*
	Vector3 dir = (getPosition() - colliderEntity->getPosition()).normalize() * 25;
	model.traslateLocal(dir.x, dir.y, dir.z);
	*/
}


//Turret
Turret::Turret() {
	hitCooldown = 0.0;
	bulletCooldown = 0.5;
	bulletTime = 0.5;
	healthPoints = 100;
	maxRange = 1570;
	minRange = 150;
};

Turret::Turret(float seconds) {
	bulletCooldown = seconds;
	bulletTime = seconds;
	healthPoints = 100;
	maxRange = 1570;
	minRange = 150;
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
			(targetPosition - cannonShift).normalize() * 500, id, 5.8, Vector3(1.0, 0.0, 0.0));
		bulletTime = bulletCooldown;
	}
}

void Turret::onBulletCollision() {
	healthPoints -= 25;
	hitCooldown = 0.1;
}

void Turret::onDynamicCollision(EntityCollider* colliderEntity) {
	//agenjo "buscar física de particulas básica"
	/*
	healthPoints -= 2;
	Vector3 front = getGlobalMatrix().topVector();
	//La torreta no es mou del seu terra
	model.traslate(-float(12) * front.x, 0.0, -float(12) * front.z);
	//Vector3 colliderFront = colliderEntity->getFront();
	*/
}

void Turret::onDeath() {
	auto it = std::find(dynamicColliders.begin(), dynamicColliders.end(), this);
	if (it == dynamicColliders.end())
	{
		std::cout << "Error removing static collider " << this->name << std::endl;
		return;
	}
	dynamicColliders.erase(it);

	parent->removeChild(this);
	delete this;
}

#endif // !ENTITY_CPP
