#include "../include/inventoryManager.hpp"
#include "../include/inventory.hpp"

int INVENTORY_SIZE = 20;

void InventoryManager::createEmtpyInventory(int _inventoryID)
{
	inventories.emplace_back(Inventory(_inventoryID, INVENTORY_SIZE));
}

void InventoryManager::deleteInventory(int _inventoryID)
{
}
