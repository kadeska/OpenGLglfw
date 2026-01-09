#include "gameObject.hpp"

//GameObject::GameObject(glm::vec3 _position, float _radius)
//{
//	position = _position;
//	radius = _radius;
//
//	collisionSphere._center = _position;
//	collisionSphere._radius = _radius;
//}

GameObject::GameObject(glm::vec3 _position)
{
	collisionSphere = new Sphere();
	setPosition(_position);
}

Sphere* GameObject::getCollisionSphere()
{
	if (!collisionSphere)
	{
		// log("model collision sphere is null", logLevel::ERROR);
		return nullptr;
	}
	return collisionSphere;
}

glm::vec3 GameObject::getPosition()
{
	return collisionSphere->_center;
}

void GameObject::setPosition(glm::vec3 _position)
{
	collisionSphere->_center = _position;
}
