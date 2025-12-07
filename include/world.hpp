#pragma once

#include <glm/glm.hpp>

#include "shader.hpp"
#include "entity/EntityCube.hpp"
#include "entity/entityChest.hpp"

class World
{
private:
	glm::vec3 playerLocation;
	bool inRangeOfInteractable = false;

	std::vector<EntityCube> entityCubeVector;
	std::vector<EntityChest> entityChestVector;

public:
	/*
	 Default constructor that will initialize the world with default parameters.
	 The default world will be a simple flat terrain with no special features.
	 Pass a Shader object pointer to be used for rendering the world.
	*/
	World(Shader*& _shader);
	/*
	 Parameterized constructor that allows setting the seed for world generation
	 and the size of the world.
	*/
	World(Shader* _shader, float seed, int _worldSize);

	void createWorld(float seed);
	void generateWorld(float seed);

	void saveWorld(std::string _filename);

	void updateWorld();

	// Add given cube entity onject to the entityCubeVector. 
	// Checks if ID = -1, as that means your trying to pass 
	// an uninisialized cube entity. EntityCube must be initialized 
	// using its parameterized constructor.
	void addCube(EntityCube _cube);

	

	void setPlayerPos(glm::vec3 _Playerpos);

	// Spawns a new EnitityCube object at the given location. 
	// This function will initialize the EntityCube object with 
	// a unique ID and use the given _pos value.
	void spawnEntityCubeAt(glm::vec3 _pos);

	// Spawns a new EntityChest object at the given position if the location is available. 
	// Used to spawn a new chest in game. 
	void spawnChestAt(glm::vec3 _pos);

	// Creates a new EntityChest object at the given position with the specified size.
	// And returns a pointer to the created EntityChest object. 
	// Used to create chests when loading a world.
	EntityChest*& createChestAt(glm::vec3 _pos, int _size);

	// Add given chest entity object to the entityChestVector.
	// Used to add chests to the array to be rendered. 
	void addChest(EntityChest*& _chest);

	bool isPositionOccupied(glm::vec3 _pos);

	glm::vec3 snapToGrid(glm::vec3& pos);

	bool checkPlayerCollisions();


	glm::vec3 getPlayerPos() { return playerLocation; }

	// Returns value of inRangeOfInteractable
	bool getInRangeOfInteracable();

	// Checks if the player is within interactRange of the given entityPosition.
	bool isInRange(glm::vec3 playerPosition, glm::vec3 entityPosition, float interactRange);

	void interactWithObjectInRange();


	std::vector<EntityCube> getEntityCubes() { return entityCubeVector; }
	std::vector<EntityChest> getEntityChests() { return entityChestVector; }


	/*
	 Destructor to clean up any resources allocated for the world.
	 Can be used to delete this world, or unload from memory if making several worlds.
	 Makle sure to save the world before destroying it.
	*/
	~World();
};

