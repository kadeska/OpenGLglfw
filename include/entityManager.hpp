#pragma once
#include <vector>

#include "entity/entity.hpp"
#include "entity/entityCube.hpp"
#include "entity/entityChest.hpp"
#include "entity/entityPlayer.hpp"
#include "entity/enemyNormal.hpp"


class EntityManager
{
private:

public:
	EntityManager();
	~EntityManager() = default;

	void createEntityCube(glm::vec3 _cubeLocation);
	void createEntityChest(glm::vec3 _cubeLocation);
	void createEntityPlayer(glm::vec3 _cubeLocation);
	void createNormalEnemy(glm::vec3 _enemyLocation);


	std::vector<EntityCube*>& getArrayOfCubes();
	std::vector<EntityChest*>& getArrayOfChests();
	std::vector<EnemyNormal*>& getArrayOfNormalEnemies();

	EntityCube* getNewCube();
	EntityChest* getNewChest();
	EnemyNormal* getNewNormalEnemy();
	EntityPlayer* getPlayer();


	// ---------------------------------------------------------------UTILITIES---------------------------------------------------------
	
	bool isPositionOccupied(glm::vec3 _pos);
	glm::vec3 snapToGrid(glm::vec3& pos);

};

