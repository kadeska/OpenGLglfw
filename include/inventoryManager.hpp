#pragma once
#include "../include/inventory.hpp"
#include <vector>

/*
 Class: InventoryManager
 ----------------
 This class manages the creation and handling of inventories within the game.
 It provides methods to create, load, and delete inventories.
*/
class InventoryManager
{
private:
	std::vector<Inventory> inventories;

public:
	InventoryManager(const int& _numberOfInventories);
	~InventoryManager() = default;

	// Creates an emtpy inventory and adds it to the inventories vector. 
	// The _outInventory parameter is set to the newly created inventory.
	// Returns -1 if creation fails, 0 on success.
	int createEmtpyInventory(const int& _inventoryID, Inventory& _outInventory);
	
	// Creates a random inventory with the specified size and adds it to the inventories vector.
	// The _outInventory parameter is set to the newly created inventory. 
	// Returns -1 if creation fails, 0 on success.
	int createRandomInventoy(const int& _inventoryID, Inventory& _outInventory, const int& _invSize);

	// Loads an inventory from a save file based on the given inventory ID.
	// Since the inventory is loaded from file, its size is determined by the file contents.
	// The _outInventory parameter is set to the newly created inventory. 
	// Returns -1 if loading fails, 0 on success.
	int loadInventoryFromFile(const int& _inventoryID, Inventory& _outInventory); 

	// Deletes the inventory with the specified inventory ID from the inventories vector.
	void deleteInventory(const int& _inventoryID);

	

	// save the given inventory to file.
	int saveInventory(Inventory& _inventory, std::string& _fileName);

	void formatInventoryDataForRender(std::vector<Item>& _items, std::string& _outData);

	const char* itemTypeToC_String(ItemType type);

private:
	// formats the inventory data for saving and sets the referanced string to the formated data. 
	void formatDataForSave(Inventory& _inventory, std::string& _outData);
	// Converts the given inventory to a string for saving to a file.
	// If it fails, returns a string "NULL"
	std::string convertInventoryToString(Inventory& _inventory);
};

