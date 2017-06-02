#ifndef ENTITY_CPP
#define ENTITY_CPP

#include "entity.h"
#include "meshmanager.h"
#include "texturemanager.h"
#include "bulletmanager.h"

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
	return model * Vector3();
}

Vector3 Entity::getGlobalPosition() {
	return getGlobalMatrix() * Vector3();
}

//carregar-se la funció i fer el .frontVector() des d'on el necessiti
Vector3 Entity::getFront() {
	return getGlobalMatrix().frontVector();
}

Vector3 Entity::getTop() {
	return getGlobalMatrix().topVector();
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
		front.vertices.push_back(getGlobalPosition());
		Vector3 frontVector = getGlobalMatrix().frontVector()*float(30.0);
		Vector3 farPoint = frontVector + getGlobalPosition();
		front.vertices.push_back(farPoint);
		//std::cout << frontVector.x << " " << frontVector.y << " " << frontVector.z << "\n";
		if (front.vertices.size() > 0) {
			glLineWidth(4);
			front.render(GL_LINES);
		}

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

void EntityCollider::onCollision() {
	/*
	for (int i = 0; i < children.size(); i++) {
		removeChild(children[i]);
	}
	*/
}

void EntityCollider::onDynamicCollision(EntityCollider* colliderEntity) {

}

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


//Drone
Drone::Drone() {
	bulletCooldown = 2.0;
	bulletTime = 2.0;
}

Drone::Drone(float seconds) {
	bulletCooldown = seconds;
	bulletTime = seconds;
}

Drone::~Drone() {};

void Drone::update(float dt) {
	bulletTime -= dt;
}

void Drone::shoot() {

	if (bulletTime <= 0) {
		BulletManager::getInstance()->createBullet(getPosition(),
			(getGlobalMatrix() * Vector3(0, 0, 1) - getPosition()).normalize() * 400, id, 2, Vector3(0.0, 0.0, 1.0));
		bulletTime = bulletCooldown;
	}
}

void Drone::onCollision() {

}

void Drone::onDynamicCollision(EntityCollider* colliderEntity) {
	//agenjo "buscar física de particulas básica"
	healthPoints -= 1;
	Vector3 front = getFront();
	model.traslateLocal(-front.x, -front.y, -front.z);
	//Vector3 colliderFront = colliderEntity->getFront();
}


//Turret
Turret::Turret() {
	bulletCooldown = 0.5;
	bulletTime = 0.5;
	healthPoints = 100;
	detectionRange = 200;
};

Turret::Turret(float seconds) {
	bulletCooldown = seconds;
	bulletTime = seconds;
	healthPoints = 100;
	detectionRange = 200;
};

Turret::~Turret() {};

void Turret::update(float dt) {
	bulletTime -= dt;
}


void Turret::shoot() 
{
	if (bulletTime <= 0) {
		BulletManager::getInstance()->createBullet(getPosition(),
			(targetPosition - getPosition()).normalize() * 400, id, 2, Vector3(1.0, 0.0, 0.0));
		bulletTime = bulletCooldown;
	}
}

void Turret::onCollision() {

}

void Turret::onDynamicCollision(EntityCollider* colliderEntity) {
	//agenjo "buscar física de particulas básica"
	healthPoints -= 2;
	Vector3 front = getFront();
	//La torreta no es mou del seu terra
	model.traslate(-float(12)*front.x, 0.0, -float(12)*front.z);
	//Vector3 colliderFront = colliderEntity->getFront();
}

/*
//Detector
Detector::Detector() {};
Detector::~Detector() {};
void Detector::update(float dt) {

}

void Detector::onCollision() {

} */

#endif // !ENTITY_CPP
