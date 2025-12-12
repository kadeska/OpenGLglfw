#pragma once

#include <glm/glm.hpp>

#include "shader.hpp"
#include "entityManager.hpp"
#include "enemyController.hpp"
#include "entity/EntityCube.hpp"
#include "entity/entityChest.hpp"

#include <vector>



class World
{
private:
	EntityManager* entityManager = nullptr;
	EnemyController* enemyController = nullptr;
	EntityChest* closestChest = nullptr;


	glm::vec3 playerPosition;
	bool inRangeOfInteractable = false;
	bool shouldRenderInventory = false;
	bool shouldRenderPlayerInventory = false;

	void init(Shader* _shader, float _seed, int _worldSize);



	void spawnPlayer(glm::vec3 _pos);

	float getDistance(const glm::vec3& pos1, const glm::vec3& pos2);

	bool checkForClosestInteractable();

public:

	bool showInventory = false;


	/*
	 Parameterized constructor that allows setting the seed for world generation
	 and the size of the world.
	*/
	World(Shader* _shader, float _seed, int _worldSize);

	void createWorld(float seed);

	void generateWorld(float seed);

	void createCube(glm::vec3 _pos);

	// Spawns a new EntityChest object at the given position if the 
    // location is available. 
    // Used to spawn a new chest in game. 
	void createChest(glm::vec3 _pos);

	void saveWorld(std::string _filename);

	void updateWorld();

	void setPlayerPos(glm::vec3 _Playerpos);
	glm::vec3 getPlayerPos() { return playerPosition; }

	void togglePlayerInventory();

	bool checkPlayerCollisions();

	

	// Returns value of inRangeOfInteractable
	bool getInRangeOfInteracable();

	bool getShouldRenderInventory();

	bool getShoudRenderPlayerInventory();


	EntityChest* getClosestChest();

	// Checks if the player is within interactRange of the given entityPosition.
	bool isInRange(glm::vec3 playerPosition, glm::vec3 entityPosition, float interactRange);


	std::vector<EntityCube*> getArrayOfCubes();
	std::vector<EntityChest*> getArrayOfChests();
	//getArrayOf__();
	EntityManager* getEntityManager();


	/*
	 Destructor to clean up any resources allocated for the world.
	 Can be used to delete this world, or unload from memory if making several worlds.
	 Makle sure to save the world before destroying it.
	*/
	~World();
};

