#include "../include/world.hpp"
#include "../include/vertexData.hpp"
#include "../include/worldData.hpp"
#include "../include/entity/EntityCube.hpp"

#include "../include/programLogger.hpp"

#include <glm/glm.hpp>

using ProgramLogger::log;
using ProgramLogger::LogLevel;


std::vector<EntityCube> entityCubeVector = std::vector<EntityCube>();

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
	int cubeID = 0;
	for (int x = 0; x < worldSize; x++)
	{
		for (int y = 0; y < worldSize; y++)
		{
			for (int z = 0; z < worldSize; z++)
			{
				log("Creating cube at position: (" + std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z) + ")", LogLevel::DEBUG);
				log("Cube ID: " + std::to_string(cubeID), LogLevel::DEBUG);

				// Create a unique ID for each cube
				entityCubeVector.emplace_back(EntityCube(cubeID, glm::vec3(static_cast<float>(x), static_cast<float>(y), static_cast<float>(z))));
				// Add the position to the worldShader vertex data cube positions
				worldShader->vertData.cubePositions.emplace_back(entityCubeVector.back().getEntityPosition());
				cubeID++;
			}
		}
	}
}

void World::updateWorld()
{
	// Placeholder for future world update logic
	//log("World update");
}

World::~World()
{
}
