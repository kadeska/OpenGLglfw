#include "collisionManager.hpp"


bool CollisionManager::checkForCollisions(GameObject _gameObject1, GameObject _gameObject2)
{
	bool success = CollisionTests::sphereToPlane(_gameObject1.getCollisionSphere(), _gameObject1.getPosition(), glm::vec3(0,1,0));
	if (success) 
	{
		return true;
	}
	return false;
}
