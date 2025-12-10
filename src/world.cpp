#include <cmath>

#include "../include/world.hpp"
#include "../include/programLogger.hpp"
#include "../include/vertexData.hpp"

#include "../include/inventoryManager.hpp"
InventoryManager* inventoryManager = new InventoryManager(10);

using ProgramLogger::log;
using ProgramLogger::LogLevel;

static const float INTERACT_RANGE = 1.6f;
static const int CHEST_INVENTORY_SIZE = 4;


// global cube ID counter
int objectID = 0;


int lastInteractedObjectID = -1;
bool previouslyInRange = false;

Shader* worldShader;

int worldID = 0;
float worldSeed = 0.0f;
int worldSize = 0;


World::World(Shader*& _shader) 
{
	worldShader = _shader;
	entityCubeVector = std::vector<EntityCube>();
	entityChestVector = std::vector<EntityChest*>();

}

World::World(Shader* _shader, float seed, int _worldSize) 
{
	worldShader = _shader;
	worldSeed = seed;
	worldSize = _worldSize;
	entityCubeVector = std::vector<EntityCube>();
	entityChestVector = std::vector<EntityChest*>();

}

void World::createCube(int x, int y, int z) {
	log("Creating cube at position: (" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")", LogLevel::DEBUG);
	log("Cube ID: " + std::to_string(objectID), LogLevel::DEBUG);
	addCube(EntityCube(objectID++, glm::vec3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z))));
}

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
					createCube(x, y, z);
				}
			}
		}
	}
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
	if (!checkForClosestInteractable()) 
	{
		inRangeOfInteractable = false;
		shouldRenderInventory = false;
	} // not performant to runn every update call/frame

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
		if (isPositionOccupied(checkPos)) {
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
	for (EntityChest* chest : getEntityChests()) 
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
		addCube(EntityCube(objectID, snappedPos));
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

	for (EntityChest* chest : entityChestVector)
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
	float distanceSquared = glm::distance(snapToGrid(playerPosition), entityPosition);

	// Compare the squared distance to the square of the interaction range to avoid square root
	return distanceSquared <= (interactRange * interactRange);
}

//void World::interactWithObjectInRange(std::string& _outData)
//{
//	if (getInRangeOfInteracable() && closestChest->getEntityID() != -1)
//	{
//		//interactedObjectID = closestChest->getEntityID();
//		_outData.clear();
//		log("Interacting with object ID: " + std::to_string(closestChest->getEntityID()));
//		// Perform interaction logic here
//
//		//log("Chest inventory contents for inventory ID: " + std::to_string(closestChest->getChestInventory().getInventoryID()));
//		for (Item& item : closestChest->getChestInventoryItems()) 
//		{
//			log("Item ID: " + std::to_string(item.getItemID()) + "  Item quantity: " + std::to_string(item.getItemQuantity()));
//			//log("Item type: " + item.getItemType());
//			_outData += "Item ID: " + std::to_string(item.getItemID()) + "  Item quantity: " + std::to_string(item.getItemQuantity()) + "\n";
//		}
//		// toggle the inventory display state
//		//closestChest->getChestInventory().setShowInv(!closestChest->getChestInventory().canShowInventory());
//
//		showInventory = !showInventory;
//	}
//	
//}

void World::spawnChestAt(glm::vec3 _pos)
{
	glm::vec3 snappedPos = snapToGrid(_pos);
	if (!isPositionOccupied(snappedPos))
	{
		log("Spawning chest");

		// Fix: Store the pointer in a local variable, then pass the reference to addChest
		//chestPtr = createChestAt(snappedPos, 5);
		//addChest(chestPtr);
		addChest(createChestAt(snappedPos, CHEST_INVENTORY_SIZE));

		objectID++;
	}
}

void World::addChest(EntityChest*& _chest)
{
	if (_chest->getEntityID() != -1)
	{
		if (!isPositionOccupied(_chest->getEntityPosition()))
		{
			entityChestVector.push_back(_chest);
			worldShader->vertData.cubePositions.push_back(_chest->getEntityPosition());
		}
	}
}

EntityChest*& World::createChestAt(glm::vec3 _pos, int _size)
{
	newChest = new EntityChest(objectID++, _size, _pos, std::to_string(objectID) + ".inventory", inventoryManager);

	//if (newChest->getChestInventory().getInventoryID() == -1)
	//{
	//	log("OOPS!! Chest inventory was not initialized.", LogLevel::ERROR);   // This block of code should never be hit. I think its safe to comment it out.
	//	return newChest;
	//}
	
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
