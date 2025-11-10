#pragma once


#include "../include/shader.hpp"
#include "../include/entity/EntityCube.hpp"


class World
{
public:
	/*
	 Default constructor that will initialize the world with default parameters.
	 The default world will be a simple flat terrain with no special features.
	 Pass a Shader object pointer to be used for rendering the world.
	*/
	World(Shader* _shader);
	/*
	 Parameterized constructor that allows setting the seed for world generation
	 and the size of the world.
	*/
	World(Shader* _shader, float seed, int _worldSize);

	void createWorld(float seed);

	void saveWorld(const char* _filename);

	void updateWorld();

	// Add given cube entity onject to the entityCubeVector. 
	// Checks if ID = -1, as that means your trying to pass 
	// an uninisialized cube entity. EntityCube must be initialized 
	// using its parameterized constructor.
	void addCube(EntityCube _cube);

	// Spawns a new EnitityCube object at the given location. 
	// This function will inisialize the EntityCube object with 
	// a unique ID and use the given _pos value.
	void spawnEntityCubeAt(glm::vec3 _pos);


	/*
	 Destructor to clean up any resources allocated for the world.
	 Can be used to delete this world, or unload from memory if making several worlds.
	 Makle sure to save the world before destroying it.
	*/
	~World();
};

