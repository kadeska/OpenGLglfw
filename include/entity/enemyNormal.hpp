#pragma once
#include "enemyBase.hpp"


class EnemyNormal : public EnemyBase
{
public:
	EnemyNormal(glm::vec3 _location);
	~EnemyNormal() = default;

	// this function should use some algorithm set an ordered array of locations (nodes) 
	void pathfind();
	// this function will move this enemy to the next node (location)
	void move();
	// this function will attack the target 
	void attack();

};

