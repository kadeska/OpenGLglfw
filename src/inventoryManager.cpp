#include "../include/inventoryManager.hpp"
#include "../include/inventory.hpp"

const int INVENTORY_SIZE = 20;

InventoryManager::InventoryManager(const int& _numberOfInventories)
{
	inventories = std::vector<Inventory>(_numberOfInventories);
}

void InventoryManager::createEmtpyInventory(const int& _inventoryID)
{
	// call our own function for adding to the vector. 
	// Check if the vector needs to be resized and if so, do it ourselfes. 
	inventories.emplace_back(_inventoryID, INVENTORY_SIZE);
}

void InventoryManager::deleteInventory(const int& _inventoryID)
{
}
