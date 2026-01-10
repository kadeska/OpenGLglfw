#pragma once
#include <vector>
#include "../physics/gameObject.hpp"

struct WorldData 
{
	std::string saveFilePath;
	std::vector<GameObject*>* gameObjects;
};

class World
{
public:
	World();
	~World() = default;
	void update();

	WorldData getWorldData() { return worldData; }
	void setWorldData(WorldData& _worldData) { worldData = _worldData; }
	std::vector<GameObject*>& getGameObjects() { return *worldData.gameObjects; }

private:
	WorldData worldData;
};

