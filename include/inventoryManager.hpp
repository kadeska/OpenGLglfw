#pragma once
#include "../include/inventory.hpp"

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
	InventoryManager() = default;
	~InventoryManager() = default;

	void createEmtpyInventory(int _inventoryID);
	void deleteInventory(int _inventoryID);
};

