#pragma once
#include <vector>

#include "entity/entity.hpp"
#include "entity/entityCube.hpp"
#include "entity/entityChest.hpp"
#include "entity/entityPlayer.hpp"


class EntityManager
{
private:

public:
	EntityManager();
	~EntityManager() = default;

	void createEntityCube(glm::vec3 _cubeLocation);
	void createEntityChest(glm::vec3 _cubeLocation);
	void createEntityPlayer(glm::vec3 _cubeLocation);


	std::vector<EntityCube*> getArrayOfCubes();
	std::vector<EntityChest*> getArrayOfChests();

	EntityCube* getNewCube();
	EntityChest* getNewChest();
	EntityPlayer* getPlayer();


	// ---------------------------------------------------------------UTILITIES---------------------------------------------------------
	
	bool isPositionOccupied(glm::vec3 _pos);
	glm::vec3 snapToGrid(glm::vec3& pos);

};

