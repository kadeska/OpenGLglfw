#include <glm/fwd.hpp>

#include "../include/entity/entity.hpp"
#include "../include/entity/entityCube.hpp"
#include "../include/entity/entityChest.hpp"
#include "../include/entity/entityPlayer.hpp"

#include "../include/entity/enemyBase.hpp"
#include "../include/entity/enemyNormal.hpp"


#include "../include/entityManager.hpp"

#include "../include/programLogger.hpp"

using ProgramLogger::log;
using ProgramLogger::LogLevel;

InventoryManager* inventoryManager = nullptr;

std::string inventoryFileExtension = ".inv";
static const int DEFAULT_INVENTORY_SIZE = 3;

EntityCube* newCube = nullptr;
EntityChest* newChest = nullptr;
EnemyNormal* newNormalEnemy = nullptr;
EntityPlayer* player = nullptr;


std::vector<EntityCube*> enitityCubeList = std::vector<EntityCube*>();
std::vector<EntityChest*> enitityChestList = std::vector<EntityChest*>();
std::vector<EnemyNormal*> normalEnemyList = std::vector<EnemyNormal*>();

glm::vec3 playerLocation;
int entityIDcounter = 0;

EntityManager::EntityManager()
{
	log("EntityManager Constructor");
	inventoryManager = new InventoryManager(10);

	// for testing

	//EnemyBase enemy = EnemyBase();
	//enemy.init(1, 100.0f, 15.0f, 1.1f, 0.8f);
	//EnemyNormal normalEnemy = EnemyNormal(glm::vec3{5,5,5});

}

void EntityManager::createEntityCube(glm::vec3 _cubeLocation)
{
	glm::vec3 snappedPos = snapToGrid(_cubeLocation);
	if (!isPositionOccupied(snappedPos))
	{
		//log("Spawning cube");
		newCube = new EntityCube(entityIDcounter++, snappedPos);
		enitityCubeList.push_back(newCube);
	}
}

void EntityManager::createEntityChest(glm::vec3 _chestLocation)
{
	glm::vec3 snappedPos = snapToGrid(_chestLocation);
	if (!isPositionOccupied(snappedPos))
	{
		log("Spawning chest");
		newChest = new EntityChest(entityIDcounter++, DEFAULT_INVENTORY_SIZE, snappedPos, std::to_string(entityIDcounter) + inventoryFileExtension, inventoryManager);
		enitityChestList.push_back(newChest);
	}
}

void EntityManager::createEntityPlayer(glm::vec3 _playerLocation)
{
	
	glm::vec3 snappedPos = snapToGrid(_playerLocation);
	if (!isPositionOccupied(snappedPos))
	{
		log("Spawning player");
		player = new EntityPlayer(snappedPos);
		playerLocation = snappedPos;
	}
}

void EntityManager::createNormalEnemy(glm::vec3 _enemyLocation)
{
	glm::vec3 snappedPos = snapToGrid(_enemyLocation);
	if (!isPositionOccupied(snappedPos)) 
	{
		log("Creating a normal enemy");
		newNormalEnemy = new EnemyNormal(snappedPos);
		newNormalEnemy->setEntityName("zombie");
		normalEnemyList.push_back(newNormalEnemy);
	}
}

std::vector<EntityCube*>& EntityManager::getArrayOfCubes()
{
	return enitityCubeList;
}

std::vector<EntityChest*>& EntityManager::getArrayOfChests()
{
	return enitityChestList;
}

std::vector<EnemyNormal*>& EntityManager::getArrayOfNormalEnemies()
{
	return normalEnemyList;
}

EntityCube* EntityManager::getNewCube()
{
	return newCube;
}

EntityChest* EntityManager::getNewChest()
{
	return newChest;
}

EnemyNormal* EntityManager::getNewNormalEnemy()
{
	return newNormalEnemy;
}

EntityPlayer* EntityManager::getPlayer()
{
	return player;
}



// ---------------------------------------------------------------UTILITIES---------------------------------------------------------

bool EntityManager::isPositionOccupied(glm::vec3 _pos)
{
	int X1 = static_cast<int>(_pos.x);
	int Y1 = static_cast<int>(_pos.y);
	int Z1 = static_cast<int>(_pos.z);

	int X2, Y2, Z2;

	for (EntityCube* cube : enitityCubeList)
	{
		X2 = static_cast<int>(cube->getEntityPosition().x);
		Y2 = static_cast<int>(cube->getEntityPosition().y);
		Z2 = static_cast<int>(cube->getEntityPosition().z);

		if (X1 == X2 && Y1 == Y2 && Z1 == Z2)
		{
			log("Position is already occupied.", LogLevel::DEBUG);
			return true;
		}
		/*if (cube.getEntityPosition() == _pos)
		{
			log("Position is already occupied.");
			return true;
		}*/
	}

	for (EntityChest* chest : enitityChestList)
	{
		X2 = static_cast<int>(chest->getEntityPosition().x);
		Y2 = static_cast<int>(chest->getEntityPosition().y);
		Z2 = static_cast<int>(chest->getEntityPosition().z);
		if (X1 == X2 && Y1 == Y2 && Z1 == Z2)
		{
			log("Position is already occupied.", LogLevel::DEBUG);
			return true;
		}
	}
	return false;
}

glm::vec3 EntityManager::snapToGrid(glm::vec3& pos) {
	return glm::vec3(
		std::round(pos.x),
		std::round(pos.y),
		std::round(pos.z)
	);
}