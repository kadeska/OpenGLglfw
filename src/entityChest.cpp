#include "../include/entity/entityChest.hpp"

#include "../include/programLogger.hpp"
using ProgramLogger::log;
using ProgramLogger::LogLevel;

InventoryManager* invMan = nullptr;


EntityChest::EntityChest(const int& _id, const int& _size, const glm::vec3& _pos, const std::string& _inventoryFilename, InventoryManager*& _invMan)
{
	log("creating EntityChest with ID: " + std::to_string(_id));
	setEntityID(_id);
	setEntityPosition(_pos);
	inventorySize = _size;
	inventoryFilename = _inventoryFilename;
	invMan = _invMan;
	invMan->createRandomInventoy(_id, chestInventory, _size);
	saveInventory(chestInventory);

}

unsigned int EntityChest::getInventorySize()
{
	if(chestInventory.getInventorySize() == 0)
	{
		log("Inventory size is zero! Have you initialized it yet?", LogLevel::ERROR);
		return -1;
	}
	return chestInventory.getInventorySize();
}

bool EntityChest::openInventory(std::string& _inventoryData)
{
	if (!isInvOpen) { 
		log("Opening inventory");
		isInvOpen = true;
		_inventoryData.clear();
		int slotNumber = 1;
		invMan->formatInventoryDataForRender(getChestInventoryItems(), _inventoryData);
		return true;
	}
	log("Closing inventory");
	isInvOpen = false;
	return false;
}

void EntityChest::saveInventory(Inventory& _inventory)
{
	log("Saving inventory of ID: " + std::to_string(_inventory.getInventoryID()));
	invMan->saveInventory(_inventory, inventoryFilename);
}

int EntityChest::getTextureID()
{
	return texID;
}

Inventory& EntityChest::getChestInventory()
{
	return chestInventory;
}

std::vector<Item>& EntityChest::getChestInventoryItems()
{
	if (getChestInventory().getInventorySize() == 0) 
	{
		log("Inventory size is zero!", LogLevel::ERROR);
		static std::vector<Item> emptyItems;
		return emptyItems;
	}
	return getChestInventory().getInventoryArray();
}

void EntityChest::setInteractable(bool _interactable)
{
	interactable = _interactable;
}

bool EntityChest::getInteractable()
{
	return interactable;
}