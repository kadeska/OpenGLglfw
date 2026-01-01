#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

#include "camera3D.hpp"

#include "../misc/programLogger.hpp"
using ProgramLogger::log;
using ProgramLogger::LogLevel;
// log("-- Constructor", LogLevel::DEBUG);


// camera attibutes

// Default camera values.
const float YAW = 45.0f;
const float PITCH = -30.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

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
float collisionRadius = 0.4f;
float interactRadius = 1.3f;

// physics attributes

glm::vec3 velocity = glm::vec3(0.0f);
bool useGravity = false;
float gravity = -4.5f;
bool onGround = false;


Camera3D::Camera3D(glm::vec3 _pos, glm::vec3 _up, const float _yaw, const float _pitch)
{
	log("Camera3D Constructor1", LogLevel::DEBUG);

	camFront = glm::vec3(0.0f, 0.0f, -1.0f);
	camMovementSpeed = SPEED;
	camMouseSensitivity = SENSITIVITY;
	camZoomLevel = ZOOM;

	camPos = _pos;
	worldUp = _up;
	camYaw = _yaw;
	camPitch = _pitch;
	updateCameraVectors();
}

Camera3D::Camera3D(float _posX, float _posY, float _posZ, float _upX, float _upY, float _upZ, float _yaw, float _pitch)
{
	log("Camera3D Constructor2", LogLevel::DEBUG);

	camFront = glm::vec3(0.0f, 0.0f, -1.0f);
	camMovementSpeed = SPEED;
	camMouseSensitivity = SENSITIVITY;
	camZoomLevel = ZOOM;

	camPos = glm::vec3(_posX, _posY, _posZ);
	worldUp = glm::vec3(_upX, _upY, _upZ);
	camYaw = _yaw;
	camPitch = _pitch;
	updateCameraVectors();
}

glm::mat4 Camera3D::GetViewMatrix()
{
	return glm::lookAt(camPos, camPos + camFront, camUp);
}

void Camera3D::ProcessMouseMovement(float _xOffset, float _yOffset, GLboolean constrainPitch)
{
	_xOffset *= camMouseSensitivity;
	_yOffset *= camMouseSensitivity;

	camYaw += _xOffset;
	camPitch += _yOffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (camPitch > 89.0f)
			camPitch = 89.0f;
		if (camPitch < -89.0f)
			camPitch = -89.0f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void Camera3D::ProcessMouseScroll(float _yOffset)
{
	camZoomLevel -= (float)_yOffset;
	if (camZoomLevel < 1.0f)
		camZoomLevel = 1.0f;
	if(camZoomLevel > 45.0f)
		camZoomLevel = 45.0f;
}

void Camera3D::applyGravity(float _deltaTime)
{
	if (!useGravity) { velocity.y = 0.0f; return; }
	if (!onGround) 
	{
		velocity.y += gravity * _deltaTime;
	}
	else {
		velocity.y = 0.0f;
	}
}

void Camera3D::updatePosition(float _deltaTime)
{
	camPos += velocity * _deltaTime;

	// Simple ground collision at y = 0
	if (camPos.y <= 0.0f) {
		camPos.y = 0.0f;
		onGround = true;
		velocity.y = 0.0f;
	}
	else {
		onGround = false;
	}
}

void Camera3D::updateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 front = glm::vec3(0.0f, 0.0f, 0.0f);
	front.x = cos(glm::radians(camYaw)) * cos(glm::radians(camPitch));
	front.y = sin(glm::radians(camPitch));
	front.z = sin(glm::radians(camYaw)) * cos(glm::radians(camPitch));
	camFront = glm::normalize(front);
	// also re-calculate the Right and Up vector
	camRight = glm::normalize(glm::cross(camFront, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	camUp = glm::normalize(glm::cross(camRight, camFront));
}


glm::vec3 Camera3D::getCamPos()
{
	return camPos;
}

void Camera3D::setCamPos(glm::vec3 _newPos)
{
	camPos = _newPos;
}

glm::vec3 Camera3D::getCamFront()
{
	return camFront;
}

glm::vec3 Camera3D::getCamUp()
{
	return camUp;
}

glm::vec3 Camera3D::getCamRight()
{
	return camRight;
}

glm::vec3 Camera3D::getWorldUp()
{
	return worldUp;
}

float Camera3D::getCamZoom()
{
	return camZoomLevel;
}

bool Camera3D::getUseGravity()
{
	return useGravity;
}

void Camera3D::setUseGravity(bool _useGravity)
{
	useGravity = _useGravity;
}

float Camera3D::getCamMovementSpeed()
{
	return camMovementSpeed;
}

float Camera3D::getCamMouseSensitivity()
{
	return camMouseSensitivity;
}

float Camera3D::getCollisionRadius()
{
	return collisionRadius;
}

float Camera3D::getInteractionRange()
{
	return interactRadius;
}

