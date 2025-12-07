
#include <algorithm>
#include "../include/inventory.hpp"

#include "../include/programLogger.hpp"
using ProgramLogger::log;
using ProgramLogger::LogLevel;

Inventory::Inventory(const int& _id, const int& _size)
{
	if (_size == 0) 
	{
		log("Inventory size can not be zero.", LogLevel::ERROR);
		return;
	}
	// Inisialize inventory with given ID and size

	inventoryID = _id;
	inventorySize = _size;

	// Resize inventory array to given size with empty items

	inventoryArray.resize(inventorySize, Item{});
}
std::vector<Item>& Inventory::getInventoryArray()
{
	return inventoryArray;
}

void Inventory::addItem(const Item& _item)
{
    bool itemAdded = false; // Flag to check if item was added or quantity incremented

    for (int i = 0; i < getInventorySize(); i++)
    {
        // Check if the slot is empty
        if (getInventoryArray().at(i).getItemID() == 0)
        {
            // Set the empty item to be the given _item
            getInventoryArray().at(i) = _item;
            getInventoryArray().at(i).setItemQuantity(1);
            itemAdded = true;
            log("Adding item to inventory...  Item ID: " + std::to_string(_item.getItemID()) 
                + " Inventory ID: " + std::to_string(getInventoryID()));
            break; // Exit loop since the item was added
        }

        // Check if the item types match
        if (getInventoryArray().at(i).getItemType() == _item.getItemType())
        {
            // Increment quantity
            getInventoryArray().at(i).setItemQuantity(getInventoryArray().at(i).getItemQuantity() + 1);
            itemAdded = true;
            log("Incrementing item...  Item ID: " + std::to_string(_item.getItemID())
                + " Inventory ID: " + std::to_string(getInventoryID()));
            break; // Exit loop since quantity was incremented
        }
    }

    // After checking all items, if no item was added, log a warning
    if (!itemAdded)
    {
        log("Not enough room in inventory for item...  Item ID: " + std::to_string(_item.getItemID())
            + " Inventory ID: " + std::to_string(getInventoryID()), LogLevel::WARNING);
    }
}

void Inventory::removeItem(const Item& _item)
{
	inventoryArray.erase(std::remove_if(inventoryArray.begin(), inventoryArray.end(),
		[&](const Item& item) {
			return item.getItemID() == _item.getItemID();
		}), inventoryArray.end());
}

int Inventory::getInventoryID()
{
	return inventoryID;
}
int Inventory::getInventorySize()
{
	return inventorySize;
}
bool Inventory::canShowInventory()
{
	return showInventory;
}

void Inventory::setShowInv(bool _showInv)
{
	showInventory = _showInv;
	//log("showInventory: " + std::to_string(showInventory));
}