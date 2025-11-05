#pragma once

#include "../include/shader.hpp"

class World
{
public:
	/*
	 Default constructor that will initialize the world with default parameters.
	 The default world will be a simple flat terrain with no special features.
	*/
	World(Shader* _shader);
	/*
	 Parameterized constructor that allows setting the seed for world generation
	 and the size of the world.
	*/
	World(Shader* _shader, float seed, int _worldSize);

	void createWorld(float seed);

	void saveWorld(const char* _filename);

	/*
	 Destructor to clean up any resources allocated for the world.
	 Can be used to delete this world, or unload from memory if making several worlds.
	 Makle sure to save the world before destroying it.
	*/
	~World();
};

