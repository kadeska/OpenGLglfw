#pragma once
#include "collisionTests.hpp"
#include "../model/model.hpp"
#include "../render/renderable.hpp"

// the physics repersentation of the model
class GameObject
{
private:
	//Model* gameObjectModelPtr = nullptr;
	Sphere* collisionSphere = nullptr;
	AABB aabb;
	bool useGravity = false;
	glm::vec3 velocity;
	Renderable::Renderable* gameObjectRenderable = nullptr;

public:
	GameObject(glm::vec3 _position, Renderable::Renderable* _renderable);
	~GameObject() = default;
	glm::vec3 getPosition();
	void setPosition(glm::vec3 _position);

	// returns the collision sphere, or nullptr
	Sphere* getCollisionSphere();
	AABB getAABB();
	bool getUseGravity() { return useGravity; }
	void setUseGravity(bool _useGravity) { useGravity = _useGravity; }
	Renderable::Renderable* getRenderable() { return gameObjectRenderable; }
	glm::vec3 getVelocity() { return velocity; }
	void setVelocity(glm::vec3 _velocity) { velocity = _velocity; }
	
};

