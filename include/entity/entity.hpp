#pragma once
#include <string>
#include <glm/glm.hpp>


#include "../programLogger.hpp"

using ProgramLogger::log;
using ProgramLogger::LogLevel;


class BaseEntity 
{
private:
	int ID = -1;
	std::string entityName = "";
	glm::vec3 entityPosition;
	bool useGravity = true;
	int texID = 3;

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

	int getTexID();
	



	~BaseEntity();
};

