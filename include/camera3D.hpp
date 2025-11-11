#pragma once



#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//#include "world.hpp"

enum Camera_Movement 
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT
};

// Default camera values.
const float YAW = 45.0f;
const float PITCH = -30.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;




class Camera3D
{
public:
	// camera attibutes

	glm::vec3 camPos;
	glm::vec3 camFront;
	glm::vec3 camUp;
	glm::vec3 camRight;
	glm::vec3 worldUp;

	// euler angles
	float camYaw;
	float camPitch;

	// cam options
	float camMovementSpeed;
	float camMouseSensitivity;
	float camZoomLevel;
	float collisionRadius = 1.0f;

	// physics attributes

	glm::vec3 velocity = glm::vec3(0.0f);
	bool useGravity = true;
	float gravity = -4.5f;
	bool onGround = false;


	// constructor with vectors
	Camera3D(glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 _up = glm::vec3(0.0f, 1.0f, 0.0f), float _yaw = YAW, float _pitch = PITCH);

	// constructor with scalar values
	Camera3D(float _posX, float _posY, float _posZ, float _upX, float _upY, float _upZ, float _yaw, float _pitch);

	glm::mat4 GetViewMatrix();
	//void ProcessKeyboard(Camera_Movement _direction, float _deltaTime);
	void ProcessMouseMovement(float _xOffset, float _yOffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(float _yOffset);

	void applyGravity(float _deltaTime);
	void updatePosition(float _deltaTime);

private:
	void updateCameraVectors();


};

