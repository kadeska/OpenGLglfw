#include <string>

#include "../include/inventoryManager.hpp"
#include "../include/inventory.hpp"


#include "../include/programLogger.hpp"
using ProgramLogger::log;
using ProgramLogger::LogLevel;



const int INVENTORY_SIZE = 3;
unsigned int itemIDCounter = 1;


InventoryManager::InventoryManager(const int& _numberOfInventories)
{
	inventories = std::vector<Inventory>(_numberOfInventories);
}

void InventoryManager::createEmtpyInventory(const int& _inventoryID)
{
	// call our own function for adding to the vector. 
	// Check if the vector needs to be resized and if so, do it ourselfes. 
	log("[InventoryManager] Creating empty inventory with ID: " + std::to_string(_inventoryID));
	inventories.emplace_back(_inventoryID, INVENTORY_SIZE);
}

int InventoryManager::createRandomInventoy(const int& _inventoryID, Inventory& _outInventory)
{
	std::vector<Item> items = std::vector<Item>
	{
		Item{itemIDCounter++, ItemType::FOOD},
		Item{itemIDCounter++, ItemType::GOLD},
		Item{itemIDCounter++, ItemType::GOLD},
		Item{itemIDCounter++, ItemType::GOLD}
	};

	// create an inventory item array with some random items
	log("making item array.");



	// Initialize the chest inventory.
	log("Initializing chest inventory.");
	if (_outInventory.getInventorySize() == -1)
	{
		log("Can not initialize chest inventory!", LogLevel::ERROR);
		return -1;
	}
	_outInventory = Inventory(_inventoryID, INVENTORY_SIZE);

	// Add items to chest inventory.
	log("Adding items to inventory: ");

	for (Item& item : items)
	{
		log("Adding item: " + std::to_string(item.getItemID()));
		_outInventory.addItem(item);
	}

	log("Random inventory created with inventory ID: " + std::to_string(_outInventory.getInventoryID()));
	inventories.push_back(_outInventory);

	return 0;

}

void InventoryManager::deleteInventory(const int& _inventoryID)
{
}
