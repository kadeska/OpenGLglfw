#pragma once
#include "entityManager.hpp"
#include "entity/enemyBase.hpp"
#include "entity/enemyNormal.hpp"

class EnemyController
{
public:
	EnemyController(EntityManager* _entityManager);
	~EnemyController() = default;

	// Set the target of given enemy
	void setEnemyTarget(BaseEntity* _targetEntity, EnemyBase* _enemy);

	// this function will update each enemy
	void update();

private:
	EntityManager* entityManager = nullptr;
	int frameCount;
};

