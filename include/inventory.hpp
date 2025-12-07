#pragma once

#include <vector>

#include "itemTypes.hpp"
#include "item.hpp"

using ItemType = ItemTypes::ItemType;
//using ItemStruct = Item::_ItemStruct;

class Inventory
{
private:
	int inventoryID = -1;
	int inventorySize = 0;
	bool showInventory = false;

	std::vector<Item> inventoryArray;

	

public:
	Inventory() = default;
	Inventory(const int& _id, const int& _size);
	~Inventory() = default;

	std::vector<Item>& getInventoryArray();
	// Adds an item struct by ref to the inventory array
	void addItem(const Item& _item);
	void removeItem(const Item& _item);

	int getInventoryID();
	int getInventorySize();

	// returns whether the inventory should be shown or not
	bool canShowInventory();

	void setShowInv(bool _showInv);


};
