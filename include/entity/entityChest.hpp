#pragma once

#include <string>

#include "entity.hpp"
#include "../inventory.hpp"
#include "../include/inventoryManager.hpp"


class EntityChest : public BaseEntity
{
private:
	std::string inventoryFilename;
	int inventorySize = 0;
	Inventory chestInventory;
	int seed = 0;
	int texID = 2;
public:
	bool interactable = true;
	bool isInvOpen = false;
	EntityChest() = default;
	EntityChest(const int& _id, const int& _size, const glm::vec3& _pos, const std::string& _inventoryFilename, InventoryManager*& _invMan);
	~EntityChest() = default;

	int getTextureID();

	bool openInventory(std::string& _inventoryData);
	
	void saveInventory(Inventory& _inventory);

	Inventory& getChestInventory();
	unsigned int getInventorySize();
	std::vector<Item>& getChestInventoryItems();

	void setInteractable(bool _interactable);
	bool getInteractable();
};