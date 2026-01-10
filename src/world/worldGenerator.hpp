#pragma once
#include <string>
#include <vector>
#include "../world/world.hpp"
#include "../render/renderable.hpp"

class WorldGenerator
{
public:
	WorldGenerator(std::vector<Renderable::Renderable*>& _renderablesRef);
	~WorldGenerator() = default;

	void generateDefaultWorld(World*& _worldPtrRef);
	void generateWorldFromConfig(World*& _worldPtrRef, const std::string& configData);

private:
	std::vector<Renderable::Renderable*>& renderablesRef;
	//std::vector<GameObject*> gameObjects;
	WorldData* worldData = nullptr;
	std::vector<GameObject*>* gameObjects = nullptr;
	std::string path;

	void generateGameObjects();
	void addGameObject(GameObject* _gameObject);
};

