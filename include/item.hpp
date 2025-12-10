#pragma once

#include "itemTypes.hpp"
using ItemType = ItemTypes::ItemType;

// This class is a container for item struct
class Item 
{
private:
	

public:
	// This struct represents an item.
	struct _ItemStruct
	{
		unsigned int itemID = 0;
		char name[64] = "EMPTY";
		ItemType type = ItemType::EMPTY;
		unsigned int quantity = 0;
		unsigned int slotPosition = 0;
	} _item;

	inline const char* getItemName() const { return _item.name; }
	inline const unsigned int getItemID() const { return _item.itemID; }
	inline const ItemType getItemType() const { return _item.type; }
	inline const unsigned int getItemQuantity() const { return _item.quantity; }
	inline void setItemQuantity(const unsigned int& qty) { _item.quantity = qty; }
	inline unsigned int getSlotPosition() const { return _item.slotPosition; }
	inline void setSlotPosition(unsigned int _slotPosition) { _item.slotPosition = _slotPosition; };
};