#include "../include/enemyController.hpp"

EnemyController::EnemyController(EntityManager* _entityManager)
{
	log("EnemyController constructor");
	frameCount = 0;
	entityManager = _entityManager;
}

void EnemyController::setEnemyTarget(BaseEntity* _targetEntity, EnemyBase* _enemy)
{
	_enemy->setTarget(_targetEntity);
}

void EnemyController::update()
{
	frameCount++;
	//log("Frame count: " + std::to_string(frameCount));
	// every 10 frames do update logic
	if ((frameCount % 10) == 0) 
	{
		//log("Frame is multiple of 10");

		for (EnemyNormal* normalEnemy : entityManager->getArrayOfNormalEnemies()) 
		{
			// move the enemy every 10 frames
			normalEnemy->move();
		}
	}

	if ((frameCount % 500) == 0)
	{
		//log("Frame is multiple of 10");

		for (EnemyNormal* normalEnemy : entityManager->getArrayOfNormalEnemies())
		{
			// update path of each enemy every 500 frames
			normalEnemy->pathfind();
		}
	}
}
