#include "../include/entity/entityChest.hpp"

#include "../include/FileManager.hpp"
using FileManager::saveInventoryToFile;
using FileManager::readInventoryFromFile;

#include "../include/programLogger.hpp"
using ProgramLogger::log;
using ProgramLogger::LogLevel;

#include "../include/inventoryManager.hpp"
InventoryManager inventoryManager = InventoryManager(10);

EntityChest::EntityChest(const int& _id, const int& _size, const glm::vec3& _pos, const std::string& _inventoryFilename)
{
	log("creating EntityChest...");
	//generateEmptyInventory();
	setEntityID(_id);
	setEntityPosition(_pos);
	inventorySize = _size;
	inventoryFilename = _inventoryFilename;
}

int EntityChest::generateEmptyInventory()
{
	log("Creating empty inventory for object ID: " + std::to_string(getEntityID()));
	std::vector<ItemType> items = std::vector<ItemType>{ItemType::EMPTY};
	chestInventory = Inventory(getEntityID(), getInventorySize());
	log("Empty inventory created with ID: " + std::to_string(getChestInventory().getInventoryID()));
	return 0;
}


int EntityChest::generateRandomInventory()
{
	log("creating random inventoy for object ID " + std::to_string(getEntityID()));
	if (inventoryManager.createRandomInventoy(getEntityID(), chestInventory) == -1) 
	{
		log("Something went wrong");
	}
	return 0;
}

int EntityChest::generateInventoryFromFile()
{
	log("Loading inventory from file for object ID: " + std::to_string(getEntityID()));
	//// initialize an empty vector of uninisialized items (inventory items)
	//std::vector<Item> items = std::vector<Item>{};
	//// data to be written from file
	//std::string data;
	//// read inventory file and load to data string
	//readInventoryFromFile(inventoryFilename, data);
	//// finaly set the chest inventory with the data string that was read from file
	//chestInventory = Inventory(getEntityID(), (int)items.size(), items);
	//log("Inventory loaded from file with inventory ID: " + std::to_string(getChestInventory().getInventoryID()));

	return -1;
}

void EntityChest::setInventorySize(const int& _size)
{
	inventorySize = _size;
}

int EntityChest::getInventorySize()
{
	if(inventorySize == 0) 
	{
		log("Inventory size is zero! Have you initialized it yet?", LogLevel::ERROR);
		return -1;
	}
	return inventorySize;
}
void EntityChest::toggleInventory()
{
	
	if(getChestInventory().canShowInventory())
	{
		log("Opening inventory of ID: " + std::to_string(getChestInventory().getInventoryID()));
		//getChestInventory().setShowInv(false);
		//toggleInv = false;
		for (Item item : getChestInventoryItems())
		{
			log("Inventory contents: " + std::to_string(item.getItemID()));
		}
		return;
	}
	
	
}

void EntityChest::saveInventory(Inventory& _inventory)
{/*
	std::string data = convertInventoryToString(*&_inventory.getItems());
	saveInventoryToFile(inventoryFilename, data);*/
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

std::string EntityChest::convertInventoryToString(std::vector<ItemType>& items)
{
	std::string inventoryData;
	for (ItemType& item : items) {
		inventoryData += std::to_string(static_cast<int>(item)) + "\n";
	}
	return inventoryData;
}