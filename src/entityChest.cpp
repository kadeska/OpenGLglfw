#include "../include/entity/entityChest.hpp"

#include "../include/programLogger.hpp"
using ProgramLogger::log;
using ProgramLogger::LogLevel;

InventoryManager* invMan = nullptr;


EntityChest::EntityChest(const int& _id, const int& _size, const glm::vec3& _pos, const std::string& _inventoryFilename, InventoryManager*& _invMan)
{
	log("creating EntityChest with ID: " + std::to_string(_id));
	//generateEmptyInventory();
	setEntityID(_id);
	setEntityPosition(_pos);
	inventorySize = _size;
	inventoryFilename = _inventoryFilename;
	invMan = _invMan;
	invMan->createRandomInventoy(_id, chestInventory, _size);

}

unsigned int EntityChest::getInventorySize()
{
	if(chestInventory.getInventorySize() == 0)
	{
		log("Inventory size is zero! Have you initialized it yet?", LogLevel::ERROR);
		return -1;
	}
	return chestInventory.getInventorySize();
}

void EntityChest::toggleInventory()
{
	
	if(getChestInventory().canShowInventory())
	{
		log("Opening inventory of ID: " + std::to_string(getChestInventory().getInventoryID()));
		//getChestInventory().setShowInv(false);
		//toggleInv = false;
		for (Item item : getChestInventoryItems())
		{
			log("Inventory contents: " + std::to_string(item.getItemID()));
		}
		return;
	}
}

// Helper function to convert ItemType to string
const char* EntityChest::itemTypeToC_String(ItemType type)
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

bool EntityChest::openInventory(std::string& _inventoryData)
{
	if (!isInvOpen) { 
		log("Opening inventory");
		isInvOpen = true;
		_inventoryData.clear();
		int slotNumber = 1;
		for (Item& item : getChestInventoryItems())
		{
			/* int id = item.getItemID();
			 int itemQuantity = item.getItemQuantity();
			 const char* itemName = item.getItemName();
			 const char* itemID = std::to_string(id).c_str();
			 const char* itemType = itemTypeToC_String(item.getItemType());*/

			_inventoryData += "Slot: " + std::to_string(slotNumber++) + ", Name: " + item.getItemName() + ", Quantity: " + std::to_string(item.getItemQuantity()) + ", ID:" + std::to_string(item.getItemID()) + ", Type: " + itemTypeToC_String(item.getItemType()) + "\n";
			
		}
		return true;
	}
	log("Closing inventory");
	isInvOpen = false;
	
	return false;
}

void EntityChest::saveInventory(Inventory& _inventory)
{
	log("Saving inventory of ID: " + std::to_string(_inventory.getInventoryID()));
	invMan->saveInventory(_inventory);
}

int EntityChest::getTextureID()
{
	return texID;
}

Inventory& EntityChest::getChestInventory()
{
	return chestInventory;
}

std::vector<Item>& EntityChest::getChestInventoryItems()
{
	if (getChestInventory().getInventorySize() == 0) 
	{
		log("Inventory size is zero!", LogLevel::ERROR);
		static std::vector<Item> emptyItems;
		return emptyItems;
	}
	return getChestInventory().getInventoryArray();
}

void EntityChest::setInteractable(bool _interactable)
{
	interactable = _interactable;
}

bool EntityChest::getInteractable()
{
	return interactable;
}