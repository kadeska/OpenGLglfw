#pragma once

#include "itemTypes.hpp"
using ItemType = ItemTypes::ItemType;

// This class is a container for item struct
class Item 
{
public:
	// This struct represents an item.
	struct _ItemStruct
	{
		unsigned int itemID = 0;
		ItemType type = ItemType::EMPTY;
		unsigned int quantity = 0;
	} _item;

	inline const unsigned int getItemID() const { return _item.itemID; }
};