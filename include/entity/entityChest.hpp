#pragma once
#include "entity.hpp"

#include <string>





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

	Inventory& getChestInventory()
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