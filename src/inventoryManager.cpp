#include <string>

#include "../include/inventoryManager.hpp"
#include "../include/inventory.hpp"


#include "../include/programLogger.hpp"
using ProgramLogger::log;
using ProgramLogger::LogLevel;

#include "../include/FileManager.hpp"
using FileManager::saveInventoryToFile;
using FileManager::readInventoryFromFile;

unsigned int itemIDCounter = 1;

static unsigned const int __DEFAULT_INVENTORY_SIZE = 4;


InventoryManager::InventoryManager(const int& _numberOfInventories)
{
	inventories = std::vector<Inventory>(_numberOfInventories);
}

int InventoryManager::createEmtpyInventory(const int& _inventoryID, Inventory& _outInventory)
{
	if (_inventoryID == -1) 
	{
		log("Can not create inventory! Inventory ID is invalid! Can not be -1", LogLevel::ERROR);
		return -1;
	}
	log("Creating empty inventory with ID: " + std::to_string(_inventoryID));
	_outInventory = Inventory(_inventoryID, __DEFAULT_INVENTORY_SIZE);
	inventories.push_back(_outInventory);
	return 0;
}

int InventoryManager::createRandomInventoy(const int& _inventoryID, Inventory& _outInventory, const int& _invSize)
{
	if (_invSize == 0)
	{
		log("Can not initialize inventory! Inventory size can not be zero!", LogLevel::ERROR);
		return -1;
	}

	std::vector<Item> items = std::vector<Item>
	{
		Item{itemIDCounter++, "food", ItemType::FOOD},
		Item{itemIDCounter++, "gold", ItemType::GOLD},
		Item{itemIDCounter++, "gold", ItemType::GOLD},
		Item{itemIDCounter++, "gold", ItemType::GOLD}
	};
	
	_outInventory = Inventory(_inventoryID, _invSize);

	for (Item& item : items)
	{
		_outInventory.addItem(item);
	}

	log("Random inventory created with inventory ID: " + std::to_string(_outInventory.getInventoryID()));
	inventories.push_back(_outInventory);

	return 0;

}

int InventoryManager::loadInventoryFromFile(const int& _inventoryID, Inventory& _outInventory)
{
	std::string inventoryData;
	if (!readInventoryFromFile("", inventoryData))
	{
		log("Loading inventory from file failed!", LogLevel::ERROR);
		return -1;
	}
	return 0;
}

void InventoryManager::deleteInventory(const int& _inventoryID)
{
}

std::string InventoryManager::convertInventoryToString(Inventory& _inventory)
{
	if (_inventory.getInventoryID() == -1) 
	{
		log("Inventory must be valid in order to save.", LogLevel::ERROR);
		return "NULL";
	}
	std::string data;
	formatDataForSave(_inventory, data);
	return data;
}

int InventoryManager::saveInventory(Inventory& _inventory, std::string& _fileName)
{
	// check that inventory is valid
	if (_inventory.getInventoryID() == -1) 
	{
		log("Inventory must be valid to save. Inventory not being saved.", LogLevel::ERROR);
		return -1;
	}
	
	// convert the inventory to a string
	std::string inventoryData = convertInventoryToString(_inventory);

	// make sure the data is valid
	if (inventoryData == "NULL") 
	{
		log("Inventory data is invalid. Inventory not being saved.", LogLevel::ERROR);
		return -1;
	}

	// save the string to file with FileManager
	//std::string filename = std::to_string(_inventory.getInventoryID());

	return saveInventoryToFile(_fileName, inventoryData);
}

void InventoryManager::formatInventoryDataForRender(std::vector<Item>& _items, std::string& _outData)
{
	for (Item& item : _items)
	{
		_outData += "Slot: " + std::to_string(item.getSlotPosition()) +
			", Name: " + item.getItemName() +
			", Quantity: " + std::to_string(item.getItemQuantity()) +
			", ID:" + std::to_string(item.getItemID()) +
			", Type: " + itemTypeToC_String(item.getItemType()) +
			"\n";
	}
}

void InventoryManager::formatDataForSave(Inventory& _inventory, std::string& _outData)
{
	for (const Item& item : _inventory.getInventoryArray())
	{
		_outData += std::to_string(item.getSlotPosition()) + "," 
			+ std::to_string(item.getItemID()) + ","
			+ std::to_string(static_cast<int>(item.getItemType())) + ","
			+ std::to_string(item.getItemQuantity()) + "\n";
	}
}

// Helper function to convert ItemType to c style string
const char* InventoryManager::itemTypeToC_String(ItemType type)
{
	switch (type)
	{
	case ItemType::EMPTY: return "EMPTY";
	case ItemType::WOOD: return "WOOD";
	case ItemType::STONE: return "STONE";
	case ItemType::IRON: return "IRON";
	case ItemType::GOLD: return "GOLD";
	case ItemType::DIAMOND: return "DIAMOND";
	case ItemType::FOOD: return "FOOD";
	case ItemType::POTION: return "POTION";
	case ItemType::WEAPON: return "WEAPON";
	case ItemType::ARMOR: return "ARMOR";
		// Add other cases as needed
	default: return "UNKNOWN";
	}
}