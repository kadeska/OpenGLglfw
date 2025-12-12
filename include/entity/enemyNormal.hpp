#pragma once
#include "enemyBase.hpp"


class EnemyNormal : public EnemyBase
{
public:
	EnemyNormal(glm::vec3 _location);
	~EnemyNormal() = default;
};

