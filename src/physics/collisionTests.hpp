#pragma once
// thanks to https://www.miguelcasillas.com/?mcportfolio=collision-detection-c

#include <glm/glm.hpp>

struct AABB
{
	glm::vec3 _max;
	glm::vec3 _min;
};
struct Sphere
{
	glm::vec3 _center;
	float _radius;
};
struct Ray
{
	glm::vec3 _dir;
	glm::vec3 _start;
};

namespace CollisionTests
{	
	static int halfSpaceTest(const glm::vec3& _testPoint, const glm::vec3& _normal, const glm::vec3& _pointOnPlane, const float _offseset);
	static bool pointInAABB(const AABB& _box, const glm::vec3 _point);
	static bool aabbToaabb(const AABB& _box1, const AABB& _box2);
	static bool pointInSphere(const Sphere& _sphere, const glm::vec3& _point);
	static bool sphereToSphere(const Sphere& _spr1, const Sphere& _spr2);
	static bool sphereToPlane(const Sphere* _spr, const glm::vec3& _point, const glm::vec3& _normal)
	{
		//Calculate a vector from the point on the plane to the center of the sphere
		glm::vec3 temp(_spr->_center - _point);

		//Calculate the distance: dot product of the new vector with the plane's normal
		float dist(glm::dot(temp, _normal));

		if (dist > _spr->_radius)
		{
			//The sphere is not touching the plane
			return false;
		}

		//Else, the sphere is colliding with the plane
		return true;
	}
	static bool rayToSphere(const Ray& _ray, const Sphere& _sphere, float& _t);
	static bool movingSphereToSphere(const Sphere& _movingSphere, const glm::vec3& _sphereVel, const Sphere& _staticSphere, float& _t);
	static bool movingSphereToContainingSphere(const Sphere& _movingSphere, const glm::vec3& _sphereVel, const Sphere& _staticSphere, float& _t);
};

