#include <cmath>


#include "../include/programLogger.hpp"
#include "../include/vertexData.hpp"
#include "../include/entityManager.hpp"
#include "../include/enemyController.hpp"

#include "../include/world.hpp"

//#include "../include/model/model.hpp"
#include <string>


using ProgramLogger::log;
using ProgramLogger::LogLevel;

static const float INTERACT_RANGE = 1.6f;
static const int CHEST_INVENTORY_SIZE = 4;

//static const char* backpackModelPath = "models/backpack/backpack.obj";

// global cube ID counter
int objectID = 0;


int lastInteractedObjectID = -1;
bool previouslyInRange = false;

Shader* worldShader;

//Model* backpackModel = new Model(backpackModelPath);;

int worldID = 0;
float worldSeed = 0.0f;
int worldSize = 0;


World::World(Shader* _shader, float _seed, int _worldSize) 
{
	init(_shader, _seed, _worldSize);
	//setPlayerPos({ 5, 5, 5 });
	entityManager = new EntityManager();
	enemyController = new EnemyController(entityManager);
}

void World::init(Shader* _shader, float _seed, int _worldSize)
{
	worldShader = _shader;
	worldSeed = _seed;
	worldSize = _worldSize;
}


//this is the one im using 
void World::createWorld(float seed)
{
	log("Creating World: Size= " + std::to_string(worldSize) 
		+ " ID = " + std::to_string(worldID)
		+ " Seed= " + std::to_string(worldSeed)
	    );

	int wallHeight = 4;

	for (int x = 0; x < worldSize; x++) {
		for (int y = 0; y < worldSize; y++) {
			for (int z = 0; z < worldSize; z++) {
				if (y == 0 || (y < wallHeight && (x == 0 || x == worldSize - 1 || z == 0 || z == worldSize - 1))) {
					// create a cube for the world vector
					// createCube({x, y, z});
					//add the entities location to the render vector. 
					// worldShader->vertData.cubePositions.push_back(getEntityManager()->getNewCube()->getEntityPosition());
				}
			}
		}
	}


	//backpackModel = 



}

void World::generateWorld(float seed)
{
	log("Generating world...");
	const float baseHeight = 2.0f; // Set a flat base height
	const float variation = 1.6f;   // Small random variation

	for (int x = 0; x < worldSize; x++) {
		for (int y = 0; y < worldSize; y++) {
			// Create a flat terrain with slight variations
			float height = baseHeight + ((static_cast<float>(rand()) / RAND_MAX) * variation * 2) - variation;

			// Create EntityCube instances up to the generated height
			for (int z = 0; z < height; z++) {
				glm::vec3 loc = { static_cast<float>(x), static_cast<float>(z), static_cast<float>(y) };
				createCube(loc); // Assume cubeID is defined and incremented elsewhere
			}
		}
	}
	// add some chests

	/*EntityChest chest1 = EntityChest(objectID++, 5, glm::vec3(13.0f, 3.0f, 18.0f), "Chest1.txt");
	EntityChest chest2 = EntityChest(objectID++, 5, glm::vec3(21.0f, 3.0f, 9.0f), "Chest2.txt");

	addChest(chest1);
	addChest(chest2);*/

	createChest(glm::vec3(13.0f, 3.0f, 18.0f));
}

// this is the function to use to add a cube when generating a world
void World::createCube(glm::vec3 _pos) {
	entityManager->createEntityCube(_pos);
}

void World::createChest(glm::vec3 _pos)
{
	entityManager->createEntityChest(_pos);
}

void World::spawnPlayer(glm::vec3 _pos)
{
	entityManager->createEntityPlayer(_pos);
}

//have not implamented yet
void World::saveWorld(std::string _filename)
{
}

void World::updateWorld()
{
	//backpackModel->Draw(*worldShader);


	//enemyController->update();
	// check for collisions, entity updates, etc.
	checkPlayerCollisions();
	if (!checkForClosestInteractable()) 
	{
		inRangeOfInteractable = false;
		shouldRenderInventory = false;
	} // not performant to runn every update call/frame

}

void World::togglePlayerInventory()
{
	shouldRenderPlayerInventory = !shouldRenderPlayerInventory;
	log("ShouldRenderPlayerInventory = " + std::to_string(shouldRenderPlayerInventory));
}

bool World::checkPlayerCollisions()
{
	//glm::vec3 playerPosSnapped = snapToGrid(playerLocation);

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
		if (getEntityManager()->isPositionOccupied(checkPos)) {
			log("Collision detected at position: (" +
				std::to_string(checkPos.x) + ", " +
				std::to_string(checkPos.y) + ", " +
				std::to_string(checkPos.z) + ")", LogLevel::DEBUG);
			return true;
		}
	}
	return false;
}

float World::getDistance(const glm::vec3& pos1, const glm::vec3& pos2) {
	float deltaX = pos1.x - pos2.x;
	float deltaY = pos1.y - pos2.y;
	float deltaZ = pos1.z - pos2.z;
	return std::sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);
}

bool World::checkForClosestInteractable()
{
	closestChest = nullptr;
	inRangeOfInteractable = false;
	float closestDistance = INTERACT_RANGE;

	// Iterate through all chests to find the closest one within interaction range
	for (EntityChest* chest : getEntityManager()->getArrayOfChests())
	{
		// the distance between player and this chest
		float distance = glm::distance(getPlayerPos(), chest->getEntityPosition());

		if (distance <= INTERACT_RANGE) 
			// Player is in range of a chest(s)
		{
			
			if (distance < closestDistance) 
				// distance of this chest is less than previous closest chest
			{
				// set the new closest distance and chest pointer
				closestDistance = distance;
				closestChest = chest;
				inRangeOfInteractable = true;
				return true;
			}
		}
		else if((chest->isInvOpen) && distance <= INTERACT_RANGE + 0.5f)
		{
			log("Closing inventory, player walked away.");
			chest->isInvOpen = false;
		}
	}
	return false;
}

// ill keep this for now, but may not need it
void World::setPlayerPos(glm::vec3 _Playerpos)
{
	playerPosition = _Playerpos;
}

bool World::getInRangeOfInteracable()
{
	return inRangeOfInteractable;
}

bool World::getShouldRenderInventory()
{
	if ((closestChest != nullptr) && closestChest->isInvOpen) 
	{
		shouldRenderInventory = true;
	}
	else 
	{
		shouldRenderInventory = false;
	}
	return shouldRenderInventory;
}

bool World::getShoudRenderPlayerInventory()
{
	return shouldRenderPlayerInventory;
}

// maybe move this to the entity manager
EntityChest* World::getClosestChest()
{
	//checkForClosestInteractable();
	if (closestChest == nullptr) 
	{
		//log("closestChest var is nullptr.", LogLevel::ERROR);
		return nullptr;
	}

	return closestChest;
}

bool World::isInRange(glm::vec3 playerPosition, glm::vec3 entityPosition, float interactRange) {
	// Calculate the squared distance between player and entity
	float distanceSquared = glm::distance(entityManager->snapToGrid(playerPosition), entityPosition);

	// Compare the squared distance to the square of the interaction range to avoid square root
	return distanceSquared <= (interactRange * interactRange);
}

std::vector<EntityCube*> World::getArrayOfCubes()
{
	return entityManager->getArrayOfCubes();
}

std::vector<EntityChest*> World::getArrayOfChests()
{
	return entityManager->getArrayOfChests();
}

EntityManager* World::getEntityManager()
{
	return entityManager;
}

void World::test()
{
	entityManager->createNormalEnemy({ 5, 5, 5 });
	enemyController->setEnemyTarget(entityManager->getPlayer(), entityManager->getNewNormalEnemy());
}

World::~World()
{
}
