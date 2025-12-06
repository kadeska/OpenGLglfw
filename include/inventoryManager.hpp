#pragma once
#include "../include/inventory.hpp"
#include <vector>

/*
 Class: InventoryManager
 ----------------
 This class manages the creation and handling of inventories within the game.
 It provides methods to create, update, and retrieve inventories for various entities.
*/
class InventoryManager
{
private:
	std::vector<Inventory> inventories;

public:
	InventoryManager(const int& _numberOfInventories);
	~InventoryManager() = default;

	void createEmtpyInventory(const int& _inventoryID);
	void deleteInventory(const int& _inventoryID);
};

