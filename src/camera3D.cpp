#include "../include/camera3D.hpp"

Camera3D::Camera3D(glm::vec3 _pos, glm::vec3 _up, float _yaw, float _pitch) 
	: camFront(glm::vec3(0.0f, 0.0f, -1.0f)), camMovementSpeed(SPEED), 
	camMouseSensitivity(SENSITIVITY), camZoomLevel(ZOOM)
{
	camPos = _pos;
	worldUp = _up;
	camYaw = _yaw;
	camPitch = _pitch;
	updateCameraVectors();
}

Camera3D::Camera3D(float _posX, float _posY, float _posZ, float _upX, float _upY, float _upZ, float _yaw, float _pitch) 
	: camFront(glm::vec3(0.0f, 0.0f, -1.0f)), camMovementSpeed(SPEED), camMouseSensitivity(SENSITIVITY), camZoomLevel(ZOOM)
{
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

void Camera3D::ProcessKeyboard(Camera_Movement _direction, float _deltaTime)
{
	float velocity = camMovementSpeed * _deltaTime;
	if (_direction == FORWARD)
		camPos += camFront * velocity;
	if (_direction == BACKWARD)
		camPos -= camFront * velocity;
	if (_direction == LEFT)
		camPos -= camRight * velocity;
	if (_direction == RIGHT)
		camPos += camRight * velocity;
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

void Camera3D::updateCameraVectors()
{
	// calculate the new Front vector
	glm::vec3 front = glm::vec3(0.0f,0.0f,0.0f);
	front.x = cos(glm::radians(camYaw)) * cos(glm::radians(camPitch));
	front.y = sin(glm::radians(camPitch));
	front.z = sin(glm::radians(camYaw)) * cos(glm::radians(camPitch));
	camFront = glm::normalize(front);
	// also re-calculate the Right and Up vector
	camRight = glm::normalize(glm::cross(camFront, worldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	camUp = glm::normalize(glm::cross(camRight, camFront));
}
