
#include <algorithm>
#include "../include/inventory.hpp"

Inventory::Inventory(const int& _id, const int& _size)
{
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
	inventoryArray.push_back(_item);
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
bool Inventory::canShowInventory()
{
	return showInventory;
}

void Inventory::setShowInv(bool _showInv)
{
	showInventory = _showInv;
	//log("showInventory: " + std::to_string(showInventory));
}