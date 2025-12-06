#pragma once

#include <vector>

#include "itemTypes.hpp"
using ItemType = ItemTypes::ItemType;

struct Item
{
	unsigned int itemID = -1;
	ItemType type = ItemType::EMPTY;
	unsigned int quantity = 0;
};

class Inventory
{
private:
	int inventoryID = -1;
	int inventorySize = 0;
	bool showInventory = false;

	std::vector<Item> inventoryArray;

public:
	
	Inventory(int _id, int _size);
	~Inventory() = default;

	std::vector<Item> getInventoryArray();
	void addItem(ItemType& _item);
	void removeItem(ItemType& _item);

	int getInventoryID();

	// returns whether the inventory should be shown or not
	bool canShowInventory();

	void setShowInv(bool _showInv);


};
