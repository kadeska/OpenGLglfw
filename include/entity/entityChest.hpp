#pragma once
#include "entity.hpp"

#include "../include/programLogger.hpp"
using ProgramLogger::log;
using ProgramLogger::LogLevel;

#include <string>

enum class ItemType 
{
	EMPTY,
	WOOD,
	STONE,
	IRON,
	GOLD,
	DIAMOND,
	FOOD,
	POTION,
	WEAPON,
	ARMOR
};

class Inventory 
{
private:
	int inventoryID;
	int inventorySize;
	std::vector<ItemType> items;
	bool showInventory;
public:
	Inventory(int _id, int _size, std::vector<ItemType>& items);
	Inventory() = default;
	~Inventory() = default;
	inline std::vector<ItemType> getItems()
	{
		return items;
	}
	inline int getInventoryID()
	{
		return inventoryID;
	}
	// returns whether the inventory should be shown or not
	bool showInv() { return showInventory; }
	// toggles the showInventory boolean
	void setShowInv(bool _showInv) 
	{
		showInventory = _showInv;
		//log("showInventory: " + std::to_string(showInventory));
	}
};

class EntityChest : public BaseEntity
{
private:
	std::vector<ItemType> initialInv = { ItemType::EMPTY };
	Inventory chestInventory;// = Inventory(-1, 0, initialInv);
	std::string inventoryFilename;
	int inventorySize;
	int seed;
	int texID = 2;
public:
	bool interactable = true;
	EntityChest() = default;
	EntityChest(int _id, int _size, glm::vec3 _pos, std::string _inventoryFilename);
	~EntityChest() = default;

	void generateEmptyInventory();
	void generateRandomInventory();
	void generateInventoryFromFile();

	void setInventorySize(int _size);

	int getInventorySize();

	void toggleInventory();
	void saveInventory(Inventory _inventory);

	inline int getTexID()
	{
		return texID;
	}

	inline Inventory getChestInventory()
	{
		return chestInventory;
	}
	inline std::vector<ItemType> getChestInventoryItems()
	{
		return chestInventory.getItems();
	}

	inline void setInteractable(bool _interactable)
	{
		interactable = _interactable;
	}
	inline bool getInteractable()
	{
		return interactable;
	}

	inline std::string convertInventoryToString(std::vector<ItemType>& items)
	{
		std::string inventoryData;
		for (ItemType& item : items) {
			inventoryData += std::to_string(static_cast<int>(item)) + "\n";
		}
		return inventoryData;
	}
};