#pragma once
#include "gameObject.hpp"
class CollisionManager
{
	CollisionManager();
	~CollisionManager() = default;

	bool checkForCollisions(GameObject _gameObject1, GameObject _gameObject2);
};

