#pragma once
#include <glm/glm.hpp>
#include <iostream>


class BaseEntity 
{
private:
	int ID = -1;
	std::string entityName = "";
	glm::vec3 entityPosition;
	bool useGravity = false;
public:
	BaseEntity();

	void setEntityID(int _id);
	void setEntityName(std::string _name);
	void setEntityPosition(glm::vec3 _position);
	void setUseGravity(bool _useGravity);

	int getEntityID();
	std::string getEntityName();
	glm::vec3 getEntityPosition();
	bool getUseGravity();


	~BaseEntity();
};

