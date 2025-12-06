
#include <algorithm>
#include "../include/inventory.hpp"

Inventory::Inventory(int _id, int _size)
{
	inventoryID = _id;
	inventorySize = _size;
	inventoryArray.resize(inventorySize, Item{});
}
std::vector<Item> Inventory::getInventoryArray()
{
	return inventoryArray;
}

void Inventory::addItem(ItemType& _item)
{
	// if there is more than one of these items, increase quantity instead of adding new item
	// if there is not, add new item to inventory array
	
	//inventoryArray.emplace_back(_item);
}

void Inventory::removeItem(ItemType& _item)
{
	// find item with matching type and remove from quantity or remove from inventory array
	/*auto it = std::find(inventoryArray.begin(), inventoryArray.end(), _item);
	
	if (it != inventoryArray.end())
	{
		inventoryArray.erase(it);
	}*/
}

int Inventory::getInventoryID()
{
	return inventoryID;
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