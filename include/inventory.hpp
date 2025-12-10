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
	unsigned int inventorySize = 0;

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
	unsigned int getInventorySize();

};
