#include "collisionManager.hpp"


bool success = false;

CollisionManager::CollisionManager()
{
}

bool CollisionManager::checkForCollisions(GameObject* _gameObject1, GameObject* _gameObject2)
{
	success = CollisionTests::sphereToPlane(_gameObject1->getCollisionSphere(), glm::vec3(0,0,0), glm::vec3(0, 1, 0));
	if (_gameObject2 != nullptr) 
	{
		success = CollisionTests::sphereToSphere(_gameObject1->getCollisionSphere(), _gameObject2->getCollisionSphere());
	}
	


	return success;
}
