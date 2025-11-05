#pragma once
#include "entity.hpp"

class EntityCube : public BaseEntity 
{
public:
	std::string texturePath;

	EntityCube();
	EntityCube(int _id, glm::vec3 _loc);
	~EntityCube();
};

inline EntityCube::EntityCube() 
{

}

inline EntityCube::EntityCube(int _id, glm::vec3 _loc)
{
	this->setEntityID(_id);
	this->setEntityPosition(_loc);
	this->setEntityName("Block");
}

EntityCube::~EntityCube()
{
}