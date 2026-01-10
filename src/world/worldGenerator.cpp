#include "worldGenerator.hpp"
#include "../misc/programLogger.hpp"
#include "world.hpp"
using ProgramLogger::log;
using ProgramLogger::LogLevel;



WorldGenerator::WorldGenerator(std::vector<Renderable::Renderable*>& _renderablesRef)
    : renderablesRef(_renderablesRef)
{
    log("WorldGenerator constructor", LogLevel::DEBUG);
}

void WorldGenerator::generateDefaultWorld(World*& _worldPtrRef)
{
	log("Loading/generating default world...", LogLevel::DEBUG);
	World* world = new World();
	if (gameObjects == nullptr) 
	{
		gameObjects = new std::vector<GameObject*>();
		// fill gameObjects
        generateGameObjects();
	}
	if (worldData == nullptr) 
	{
		// create world data
		worldData = new WorldData("path", gameObjects);
	}
	// gameObjects has been generated so pass it to the world data
	world->setWorldData(*worldData);
	
	_worldPtrRef = world;
	return;
}

void WorldGenerator::generateGameObjects()
{
    // populate gameObjects
    log("Populating gameObjects", LogLevel::DEBUG);

    for (Renderable::Renderable* r : renderablesRef)
    {
        glm::vec3& pos = r->position;
        unsigned int id = r->model->getID();
        switch (id)
        {
        case 0:
            addGameObject(new GameObject(pos, r));
            break;
        case 1:
            addGameObject(new GameObject(pos, r));
            break;
        case 2:
            addGameObject(new GameObject(pos, r));
            break;
        default:
            break;
        }
    }
    gameObjects->at(0)->setUseGravity(true); // for testing the flag
}

void WorldGenerator::addGameObject(GameObject* _gameObject)
{
    log("Adding game object", LogLevel::DEBUG);
    gameObjects->push_back(_gameObject);
}