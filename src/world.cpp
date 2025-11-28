#include "../include/world.hpp"
#include "../include/vertexData.hpp"
#include "../include/worldData.hpp"
//#include "../include/entity/EntityCube.hpp"

#include "../include/programLogger.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>



using ProgramLogger::log;
using ProgramLogger::LogLevel;


//bool inRangeOfInteractable = false;

std::vector<EntityCube> entityCubeVector = std::vector<EntityCube>();
int cubeID = 0;


Shader* worldShader;

int worldID = 0;
float worldSeed = 0.0f;
int worldSize = 0;


World::World(Shader* _shader) 
{
	worldShader = _shader;
}

World::World(Shader* _shader, float seed, int _worldSize) 
{
	worldShader = _shader;
	worldSeed = seed;
	worldSize = _worldSize;
}

void World::createWorld(float seed)
{
	log("Creating World: Size= " + std::to_string(worldSize) 
		+ " ID = " + std::to_string(worldID)
		+ " Seed= " + std::to_string(worldSeed)
	    );
	//log("test error", LogLevel::ERROR);
	//log("test warning", LogLevel::WARNING);
	//log("test info", LogLevel::INFO);
	//int cubeID = 0;
	for (int x = 0; x < worldSize; x++)
	{
		for (int y = 0; y < worldSize; y++)
		{
			for (int z = 0; z < worldSize; z++)
			{
				log("Creating cube at position: (" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")", LogLevel::DEBUG);
				log("Cube ID: " + std::to_string(cubeID), LogLevel::DEBUG);

				// Create a cube entity at the specified position

				//addCube(EntityCube(cubeID, glm::vec3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z))));

				if(y == 0) // only create cubes at ground level for a flat world
				{
					addCube(EntityCube(cubeID, glm::vec3(static_cast<float>(x), static_cast<float>(y - 1), static_cast<float>(z))));
				}
				cubeID++;
			}
		}
	}
}

void World::updateWorld()
{
	// check for collisions, entity updates, etc.
	checkPlayerCollisions();
}

bool World::checkPlayerCollisions()
{
	// if the player is inside of a block, ignor check
	//if (isPositionOccupied(playerLocation)) { return false; }


	glm::vec3 playerPosSnapped = snapToGrid(playerLocation);

	glm::vec3 directions[6] = {
		glm::vec3(1, 0, 0),  // +X
		glm::vec3(-1, 0, 0), // -X
		glm::vec3(0, 1, 0),  // +Y
		glm::vec3(0, -1, 0), // -Y
		glm::vec3(0, 0, 1),  // +Z
		glm::vec3(0, 0, -1)  // -Z
	};


	

	// check for physical collision here

	for (int i = 0; i < 6; ++i) {
		glm::vec3 checkPos = playerPosSnapped + directions[i];
		if (isPositionOccupied(checkPos)) {
			log("Collision detected at position: (" +
				std::to_string(checkPos.x) + ", " +
				std::to_string(checkPos.y) + ", " +
				std::to_string(checkPos.z) + ")", LogLevel::DEBUG);
			return true;
		}
	}




	// check for interactibles 

	// the closest interactable entity to the player
	EntityCube closestInteractable;

	// for each cube in the world (change to be for each interactable entity in the world)
	for (EntityCube& cube : entityCubeVector) // changed to be a reference to improve performance by avoiding a copy
	{
		// if the entity is interactable. (can be removed once I make a vector of interacables)
		if (cube.getIsInteractable())
		{
			//log("Interactable");
			// check if the player is withen range of an interactable cube
			if (isInRange(playerLocation, cube.getEntityPosition(), 1.2f))
			{
				log("In range of interactable: " + std::to_string(cube.getEntityID()));
				inRangeOfInteractable = true;
				//return;
			}
			else { inRangeOfInteractable = false; }
			//else if (!isInRange(playerLocation, cube.getEntityPosition(), 1.2f))
			//{
			//	log("Not in range of interactable: " + std::to_string(cube.getEntityID()));
			//	inRangeOfInteractable = false;
			//	//return;
			//}
		}
	}

	return false;
}

void World::addCube(EntityCube _cube)
{
	// Cube ID of -1 is invalid/uninitialized. -1 is a default value for uninisialized cube entities.
	if (_cube.getEntityID() != -1) 
	{
		if (!isPositionOccupied(_cube.getEntityPosition())) 
		{
			entityCubeVector.push_back(_cube);
			worldShader->vertData.cubePositions.push_back(_cube.getEntityPosition());
		}
		
	}
}

void World::setPlayerPos(glm::vec3 _Playerpos)
{
	playerLocation = _Playerpos;
}

void World::spawnEntityCubeAt(glm::vec3 _pos) 
{
	glm::vec3 snappedPos = snapToGrid(_pos);
	if (!isPositionOccupied(snappedPos))
	{
		log("Spawning cube");
		addCube(EntityCube(cubeID, snappedPos));
		cubeID++;
	}
}

void World::spawnInteractableAt(glm::vec3 _pos)
{
	glm::vec3 snappedPos = snapToGrid(_pos);
	if (!isPositionOccupied(snappedPos))
	{
		log("Spawning interatable");
		//EntityCube interactable(cubeID, snappedPos);
		//interactable.setIsInteractable(true);
		//addCube(interactable);
		addCube(EntityCube(cubeID, snappedPos));

		for (int i = 0; i < entityCubeVector.size(); i++) 
		{
			if (entityCubeVector.at(i).getEntityID() == cubeID)
			{
				entityCubeVector.at(i).setIsInteractable(true);
			}
		}

		cubeID++;
	}
}

bool World::isPositionOccupied(glm::vec3 _pos)
{
	int X1 = static_cast<int>(_pos.x);
	int Y1 = static_cast<int>(_pos.y);
	int Z1 = static_cast<int>(_pos.z);

	int X2, Y2, Z2;

	for (EntityCube cube : entityCubeVector) 
	{
		X2 = static_cast<int>(cube.getEntityPosition().x);
		Y2 = static_cast<int>(cube.getEntityPosition().y);
		Z2 = static_cast<int>(cube.getEntityPosition().z);

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
	return false;
}


bool World::isInRangeOfInteracable()
{
	return inRangeOfInteractable;
}

bool World::isInRange(glm::vec3 playerPosition, glm::vec3 entityPosition, float interactRange) {
	// Calculate the squared distance between player and entity
	float distanceSquared = glm::distance(playerPosition, entityPosition);

	// Compare the squared distance to the square of the interaction range to avoid square root
	return distanceSquared <= (interactRange * interactRange);
}

glm::vec3 World::snapToGrid(glm::vec3& pos) {
	return glm::vec3(
		std::round(pos.x),
		std::round(pos.y),
		std::round(pos.z)
	);
}

World::~World()
{
}
