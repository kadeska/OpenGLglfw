#include "../include/entity/entity.hpp"

void BaseEntity::setEntityID(int _id)
{
	ID = _id;
}

void BaseEntity::setEntityName(std::string _name)
{
	entityName = _name;
}

void BaseEntity::setEntityPosition(glm::vec3 _position)
{
	entityPosition = _position;
}

void BaseEntity::setUseGravity(bool _useGravity)
{
	useGravity = _useGravity;
}

void BaseEntity::setIsInteractable(bool _isInteractable)
{
	isInteractable = _isInteractable;
}

int BaseEntity::getEntityID()
{
	return ID;
}

std::string BaseEntity::getEntityName()
{
	return entityName;
}

glm::vec3 BaseEntity::getEntityPosition()
{
	return entityPosition;
}

bool BaseEntity::getUseGravity()
{
	return useGravity;
}

bool BaseEntity::getIsInteractable()
{
	return isInteractable;
}

BaseEntity::BaseEntity()
{
	ID = -1;
	entityPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	entityName = "NULL";
	isInteractable = false;
}


BaseEntity::~BaseEntity()
{
}
