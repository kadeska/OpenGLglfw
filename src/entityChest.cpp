#include "../include/entity/entityChest.hpp"

#include "../include/FileManager.hpp"
using FileManager::saveInventoryToFile;
using FileManager::readInventoryFromFile;

Inventory::Inventory(int _id, int _size, std::vector<ItemType>& _items)
{
	inventoryID = _id;
	inventorySize = _size;
	items = _items;
	setShowInv(false);
	
}

EntityChest::EntityChest(int _id, int _size, glm::vec3 _pos, std::string _inventoryFilename)
{
	log("creating EntityChest...");
	//generateEmptyInventory();
	setEntityID(_id);
	setEntityPosition(_pos);
	inventorySize = _size;
	inventoryFilename = _inventoryFilename;
}

void EntityChest::generateEmptyInventory()
{
	log("Creating empty inventory for object ID: " + std::to_string(getEntityID()));
	std::vector<ItemType> items = std::vector<ItemType>{ItemType::EMPTY};
	chestInventory = Inventory(getEntityID(), (int)items.size(), items);
	log("Empty inventory created with ID: " + std::to_string(getChestInventory().getInventoryID()));
}

void EntityChest::generateRandomInventory()
{
	log("creating random inventoy for object ID " + std::to_string(getEntityID()));
	std::vector<ItemType> items = std::vector<ItemType>
	{ 
		ItemType::EMPTY,
		ItemType::FOOD,
		ItemType::GOLD
	};
	chestInventory = Inventory(getEntityID(), (int)items.size(), items);
	log("Random inventory created with inventory ID: " + std::to_string(getChestInventory().getInventoryID()));
}

void EntityChest::generateInventoryFromFile()
{
	log("Loading inventory from file for object ID: " + std::to_string(getEntityID()));
	// initialize an empty vector of items (inventory items)
	std::vector<ItemType> items = std::vector<ItemType>{ItemType::EMPTY};
	// data to be written from file
	std::string data;
	// read inventory file and load to data string
	readInventoryFromFile(inventoryFilename, data);
	// finaly set the chest inventory with the data string that was read from file
	chestInventory = Inventory(getEntityID(), (int)items.size(), items);
	log("Inventory loaded from file with inventory ID: " + std::to_string(getChestInventory().getInventoryID()));
}

void EntityChest::setInventorySize(int _size)
{
	inventorySize = _size;
}

int EntityChest::getInventorySize()
{
	if(!inventorySize) 
	{
		log("Inventory size is zero! Have you initialized it yet?", LogLevel::WARNING);
		return -1;
	}
	return inventorySize;
}
bool toggleInv = false;
void EntityChest::toggleInventory()
{
	
	if(toggleInv)
	{
		log("Closing inventory of ID: " + std::to_string(getChestInventory().getInventoryID()));
		getChestInventory().setShowInv(false);
		toggleInv = false;
		return;
	} else 
	{
		log("Opening inventory of ID: " + std::to_string(getChestInventory().getInventoryID()));
		getChestInventory().setShowInv(true);
		toggleInv = true;
		for (ItemType item : getChestInventoryItems())
		{
			log("Inventory contents: " + std::to_string((int)item));
		}
	}
	
	
}

void EntityChest::saveInventory(Inventory _inventory)
{
	std::string data = convertInventoryToString(*&_inventory.getItems());
	saveInventoryToFile(inventoryFilename, data);
}
