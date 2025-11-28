#pragma once
#include "entity.hpp"

class EntityCube : public BaseEntity 
{
public:
	//std::string texturePath;
	bool isInteractable = false;

	// default constructor. Sets ID to -1, position to (0,0,0), name to "NULL"
	EntityCube();
	// parameterized constructor. Sets ID, position, name to "Block"
	EntityCube(int _id, glm::vec3 _loc);
	~EntityCube();

	void setIsInteractable(bool _isInteractable);

	bool getIsInteractable();
};

