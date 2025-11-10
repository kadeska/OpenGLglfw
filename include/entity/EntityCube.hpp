#pragma once
#include "entity.hpp"

class EntityCube : public BaseEntity 
{
public:
	std::string texturePath;

	// default constructor. Sets ID to -1, position to (0,0,0), name to "NULL"
	EntityCube();
	// parameterized constructor. Sets ID, position, name to "Block"
	EntityCube(int _id, glm::vec3 _loc);
	~EntityCube();
};

