#pragma once
//#include <glad/glad.h>



//#include <glad/glad.h>
//#include <glm/glm.hpp>

//#include "world.hpp"

enum Camera_Movement 
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};



class Camera3D
{
public:
	


	// constructor with vectors
	Camera3D(glm::vec3 _pos, glm::vec3 _up, const float _yaw, const float _pitch);

	// constructor with scalar values
	Camera3D(float _posX, float _posY, float _posZ, float _upX, float _upY, float _upZ, float _yaw, float _pitch);

	glm::mat4 GetViewMatrix();
	//void ProcessKeyboard(Camera_Movement _direction, float _deltaTime);
	void ProcessMouseMovement(float _xOffset, float _yOffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float _yOffset);

	void applyGravity(float _deltaTime);
	void updatePosition(float _deltaTime);

	glm::vec3 getCamPos();
	void setCamPos(glm::vec3 _newPos);
	glm::vec3 getCamFront();
	glm::vec3 getCamUp();
	glm::vec3 getCamRight();
	glm::vec3 getWorldUp();

	float getCamZoom();

	bool getUseGravity();
	void setUseGravity(bool _useGravity);
	float getCamMovementSpeed();
	float getCamMouseSensitivity();
	float getCollisionRadius();
	float getInteractionRange();

private:
	void updateCameraVectors();


};

