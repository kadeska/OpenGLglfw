#include "../include/world.hpp"
#include "../include/vertexData.hpp"
#include "../include/worldData.hpp"

#include "../include/programLogger.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>


using ProgramLogger::log;
using ProgramLogger::LogLevel;


static const float INTERACT_RANGE = 1.5f;


//bool inRangeOfInteractable = false;

EntityChest closestChest;

EntityChest newChest;// = EntityChest(-1, 0, glm::vec3(0, 0, 0), "null");


// global cube ID counter
int objectID = 0;


Shader* worldShader;

int worldID = 0;
float worldSeed = 0.0f;
int worldSize = 0;


World::World(Shader* _shader) 
{
	worldShader = _shader;
	entityCubeVector = std::vector<EntityCube>();
	entityChestVector = std::vector<EntityChest>();

}

World::World(Shader* _shader, float seed, int _worldSize) 
{
	worldShader = _shader;
	worldSeed = seed;
	worldSize = _worldSize;
	entityCubeVector = std::vector<EntityCube>();
	entityChestVector = std::vector<EntityChest>();

}

void World::createWorld(float seed)
{
	log("Creating World: Size= " + std::to_string(worldSize) 
		+ " ID = " + std::to_string(worldID)
		+ " Seed= " + std::to_string(worldSeed)
	    );
	for (int x = 0; x < worldSize; x++)
	{
		for (int y = 0; y < worldSize; y++)
		{
			for (int z = 0; z < worldSize; z++)
			{
				log("Creating cube at position: (" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")", LogLevel::DEBUG);
				log("Cube ID: " + std::to_string(objectID), LogLevel::DEBUG);

				// Create a cube entity at the specified position

				//addCube(EntityCube(cubeID, glm::vec3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z))));

				if(y == 0) // only create cubes at ground level for a flat world
				{
					addCube(EntityCube(objectID, glm::vec3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z))));
				}
				objectID++;
			}
		}
	}
}

void World::generateWorld(float seed)
{
	const float baseHeight = 2.0f; // Set a flat base height
	const float variation = 1.6f;   // Small random variation

	for (int x = 0; x < worldSize; x++) {
		for (int y = 0; y < worldSize; y++) {
			// Create a flat terrain with slight variations
			float height = baseHeight + ((static_cast<float>(rand()) / RAND_MAX) * variation * 2) - variation;

			// Create EntityCube instances up to the generated height
			for (int z = 0; z < height; z++) {
				glm::vec3 loc = { static_cast<float>(x), static_cast<float>(z), static_cast<float>(y) };
				addCube(EntityCube(objectID++, loc)); // Assume cubeID is defined and incremented elsewhere
			}
		}
	}
	// add some chests

	/*EntityChest chest1 = EntityChest(objectID++, 5, glm::vec3(13.0f, 3.0f, 18.0f), "Chest1.txt");
	EntityChest chest2 = EntityChest(objectID++, 5, glm::vec3(21.0f, 3.0f, 9.0f), "Chest2.txt");

	addChest(chest1);
	addChest(chest2);*/

	spawnChestAt(glm::vec3(13.0f, 3.0f, 18.0f));
}

void World::saveWorld(std::string _filename)
{
}

void World::updateWorld()
{
	// check for collisions, entity updates, etc.
	checkPlayerCollisions();
}

bool World::checkPlayerCollisions()
{
	inRangeOfInteractable = false;

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
		glm::vec3 checkPos = getPlayerPos() + directions[i];
		if (isPositionOccupied(checkPos)) {
			log("Collision detected at position: (" +
				std::to_string(checkPos.x) + ", " +
				std::to_string(checkPos.y) + ", " +
				std::to_string(checkPos.z) + ")", LogLevel::DEBUG);
			return true;
		}
	}

	// check for interactibles 

	// for each cube in the world (change to be for each interactable entity in the world)
	for (EntityChest& chest : entityChestVector) // changed to be a reference to improve performance by avoiding a copy
	{
		// if the entity is interactable. (can be removed once I make a vector of interacables)
		if (chest.getInteractable())
		{
			//log("Interactable");
			// check if the player is withen range of an interactable cube
			if (isInRange(getPlayerPos(), chest.getEntityPosition(), INTERACT_RANGE))
			{
				//log("In range of interactable: " + std::to_string(chest.getEntityID()));
				inRangeOfInteractable = true;
				closestChest = chest;
				//return;
			}
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

void World::addChest(EntityChest& _chest) 
{
	if (_chest.getEntityID() != -1)
	{
		if (!isPositionOccupied(_chest.getEntityPosition()))
		{
			entityChestVector.push_back(_chest);
			worldShader->vertData.cubePositions.push_back(_chest.getEntityPosition());
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
		addCube(EntityCube(objectID, snappedPos));
		objectID++;
	}
}

void World::spawnChestAt(glm::vec3 _pos)
{
	glm::vec3 snappedPos = snapToGrid(_pos);
	if (!isPositionOccupied(snappedPos))
	{
		log("Spawning chest");

		addChest(createChestAt(snappedPos, 5));

		objectID++;
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

	for (EntityChest& chest : entityChestVector)
	{
		X2 = static_cast<int>(chest.getEntityPosition().x);
		Y2 = static_cast<int>(chest.getEntityPosition().y);
		Z2 = static_cast<int>(chest.getEntityPosition().z);
		if (X1 == X2 && Y1 == Y2 && Z1 == Z2)
		{
			log("Position is already occupied.", LogLevel::DEBUG);
			return true;
		}
	}
	return false;
}


bool World::getInRangeOfInteracable()
{
	return inRangeOfInteractable;
}

bool World::isInRange(glm::vec3 playerPosition, glm::vec3 entityPosition, float interactRange) {
	// Calculate the squared distance between player and entity
	float distanceSquared = glm::distance(snapToGrid(playerPosition), entityPosition);

	// Compare the squared distance to the square of the interaction range to avoid square root
	return distanceSquared <= (interactRange * interactRange);
}

void World::interactWithObjectInRange()
{
	if (getInRangeOfInteracable() && closestChest.getEntityID() != -1)
	{
		log("Interacting with object ID: " + std::to_string(closestChest.getEntityID()));
		// Perform interaction logic here
		closestChest.toggleInventory();
		//closestChest.getChestInventory().setShowInv(true);
	}
	//log("Interacting with object ID: " + std::to_string(closestInteractable->getEntityID()));
}

EntityChest& World::createChestAt(glm::vec3 _pos, int _size)
{
	newChest = EntityChest(objectID++, _size, _pos, std::to_string(objectID) + ".inventory");
	newChest.generateRandomInventory();
	return newChest;
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
