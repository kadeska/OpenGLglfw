#pragma once
#include <glm/glm.hpp>
#include <iostream>


class BaseEntity 
{
private:
	int ID = -1;
	std::string entityName = "";
	glm::vec3 entityPosition;
public:
	BaseEntity();

	void setEntityID(int _id);
	void setEntityName(std::string _name);
	void setEntityPosition(glm::vec3 _position);

	int getEntityID();
	std::string getEntityName();
	glm::vec3 getEntityPosition();


	~BaseEntity();
};

BaseEntity::BaseEntity()
{
	ID = -1;
}

inline void BaseEntity::setEntityID(int _id)
{
	ID = _id;
}

inline void BaseEntity::setEntityName(std::string _name)
{
	entityName = _name;
}

inline void BaseEntity::setEntityPosition(glm::vec3 _position)
{
	entityPosition = _position;
}

inline int BaseEntity::getEntityID()
{
	return ID;
}

inline std::string BaseEntity::getEntityName()
{
	return entityName;
}

inline glm::vec3 BaseEntity::getEntityPosition()
{
	return entityPosition;
}

inline BaseEntity::~BaseEntity()
{
}
