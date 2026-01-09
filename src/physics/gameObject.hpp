#pragma once
#include "collisionTests.hpp"
#include "../model/model.hpp"

// the physics repersentation of the model
class GameObject
{
private:
	//Model* gameObjectModelPtr = nullptr;
	Sphere* collisionSphere = nullptr;
	AABB aabb;

public:
	GameObject(glm::vec3 _position);
	~GameObject() = default;
	glm::vec3 getPosition();
	void setPosition(glm::vec3 _position);

	Sphere* getCollisionSphere();
	AABB getAABB();
	
};

