#include "../include/entity/enemyNormal.hpp"

EnemyNormal::EnemyNormal(glm::vec3 _location)
{
	log("EntityNormal Constructor");
	setEntityPosition(_location);
}

void EnemyNormal::pathfind()
{
}

void EnemyNormal::move()
{
}

void EnemyNormal::attack()
{
}
