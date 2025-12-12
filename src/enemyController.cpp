#include "../include/enemyController.hpp"

EnemyController::EnemyController(EntityManager* _entityManager)
{
	log("EnemyController constructor");
}

void EnemyController::setEnemyTarget(BaseEntity* _targetEntity, EnemyBase* _enemy)
{
	_enemy->setTarget(_targetEntity);
}
