#pragma once
#include <glm/fwd.hpp>

#include "../entity/entity.hpp"
#include "../inventory.hpp"

class EntityPlayer : public BaseEntity
{
private:
	//int textureID = 4;
	struct PlayerInfo 
	{
		float playerHealth = 100.0f;
		glm::vec3 playerLocation = glm::vec3(0,0,0);
		Inventory* playerInventory = nullptr;
	}_playerInfo;

public:
	EntityPlayer(glm::vec3 _playerLocation);
	~EntityPlayer() = default;

};

