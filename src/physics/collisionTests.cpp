#include "collisionTests.hpp"


int CollisionTests::halfSpaceTest(const glm::vec3& _testPoint, const glm::vec3& _normal, const glm::vec3& _pointOnPlane, const float _offseset)
{
	glm::vec3 temp(_testPoint - _pointOnPlane);
	float dist(glm::dot(_testPoint, _normal) - _offseset);

	if (dist > FLT_EPSILON) 
	{
		//Point is in front of the plane
		return 0;
	}
	else if (dist < -FLT_EPSILON) 
	{
		//Point is behind the plane
		return 1;
	}
	//If neither of these were true, then the point is on the plane
	return 2;
}

bool CollisionTests::pointInAABB(const AABB& _box, const glm::vec3 _point)
{
	//Check if the point is less than max and greater than min
	if (_point.x > _box._min.x && _point.x < _box._max.x &&
		_point.y > _box._min.y && _point.y < _box._max.y &&
		_point.z > _box._min.z && _point.z < _box._max.z) 
	{
		return true;
	}
	return false;
}

bool CollisionTests::aabbToaabb(const AABB& _box1, const AABB& _box2)
{
	//Check if Box1's max is greater than Box2's min and Box1's min is less than Box2's max
	return(_box1._max.x > _box2._min.x &&
		_box1._min.x < _box2._max.x &&
		_box1._max.y > _box2._min.y &&
		_box1._min.y < _box2._max.y &&
		_box1._max.z > _box2._min.z &&
		_box1._min.z < _box2._max.z);
}

bool CollisionTests::pointInSphere(const Sphere& _sphere, const glm::vec3& _point)
{
	glm::vec3 dist(_sphere._center - _point);
	float distsqr(glm::dot(dist, dist));

	// Calculate if the squared distance between the sphere's center and the point
    // is less than the squared radius of the sphere
	if (distsqr < (_sphere._radius * _sphere._radius)) 
	{
		return true;
	}
	return false;
}

bool CollisionTests::sphereToSphere(const Sphere& _spr1, const Sphere& _spr2)
{
	glm::vec3 dist(_spr1._center - _spr2._center);
	float distsqr(glm::dot(dist, dist));
	float radiiSumSquared(_spr1._radius + _spr2._radius);
	radiiSumSquared *= radiiSumSquared;

	if (distsqr <= radiiSumSquared) 
	{
		return true;
	}
	return false;
}

//bool CollisionTests::sphereToPlane(const Sphere& _spr, const glm::vec3& _point, const glm::vec3& _normal)
//{
//	//Calculate a vector from the point on the plane to the center of the sphere
//	glm::vec3 temp(_spr._center - _point);
//
//	//Calculate the distance: dot product of the new vector with the plane's normal
//	float dist(glm::dot(temp, _normal));
//
//	if (dist > _spr._radius)
//	{
//		//The sphere is not touching the plane
//		return false;
//	}
//
//	//Else, the sphere is colliding with the plane
//	return true;
//}

bool CollisionTests::rayToSphere(const Ray& _ray, const Sphere& _sphere, float& _t)
{
	//Create a vector from the sphere to the ray's start point
	glm::vec3 vec = _ray._start - _sphere._center;

	//Get the dot product of this vector with the ray's direction
	//This will become the 'b' part of our quadratic equation
	float fB = glm::dot(vec, _ray._dir);

	//Get the square distance from the start of the ray to the sphere's surface
	//This will become the 'c' part of our quadratic equation
	float fC = glm::dot(vec, vec) - (_sphere._radius * _sphere._radius);

	//If the ray starts outside the sphere and points away from it, we return false
	if (fC > 0.0f && fB > 0.0f)
		return false;

	//Else, we get the discriminant for our equation
	float fDisc = fB * fB - fC;

	//If this is less than zero, we return false
	if (fDisc < 0.0f)
		return false;

	//If we didn't want the time of collision, we could just return true here.
	//But in this case we do want it, so we'll continue

	//We solve our equation and get the time of collision
	//We use -sqrt(fDisc) to get the smallest root, ie. the first point in which the ray touches the sphere
	_t = -fB - sqrt(fDisc);

	//If the time is less than zero, it means the ray started inside the sphere
	//If so, we just make it zero
	if (_t < 0.0f)
		_t = 0.0f;

	//Our collision point is going to be:
	//tRay.m_vecStart + tRay.m_vecDir * fT
	return true;
}

bool CollisionTests::movingSphereToSphere(const Sphere& _movingSphere, const glm::vec3& _sphereVel, const Sphere& _staticSphere, float& _t)
{
	//First, we create a new sphere based on the static sphere's position but with the radius of both spheres
	Sphere tempSphere(_staticSphere);
	tempSphere._radius += _movingSphere._radius;

	//Now, we get the length of our sphere's velocity. We will use this for the ray to sphere test and to check against collision time
	float fVecLength = glm::length(_sphereVel);

	//We create a ray that starts at the moving sphere's center and has a direction of the normalized velocity
	Ray tempRay;
	tempRay._start = _movingSphere._center;
	tempRay._dir = _sphereVel / fVecLength;

	//We call our ray to sphere function,
	if (rayToSphere(tempRay, tempSphere, _t))
	{
		//We check if the time is less than or equal to the velocity's length
		if (_t <= fVecLength)
		{
			return true;
		}
	}
	return false;
}

bool CollisionTests::movingSphereToContainingSphere(const Sphere& _movingSphere, const glm::vec3& _sphereVel, const Sphere& _staticSphere, float& _t)
{
	//First we check, if the length from the static sphere's center to the final position of the moving sphere
	//is not greater or equal to the static sphere's radius minus the moving sphere's radius, then there's no collision.
	//We will use squared distances for this.
	glm::vec3 vecToFinalPost((_movingSphere._center + _sphereVel) - _staticSphere._center);
	if (glm::dot(vecToFinalPost, vecToFinalPost) < (_staticSphere._radius - _movingSphere._radius))
	{
		return false;
	}

	//If we get to this point, it means there is a collision!
	//First, we subtract the radius of the moving sphere from the static sphere's radius.
	//We will save it into a new sphere for ease of use
	Sphere tempSphere(_staticSphere);
	tempSphere._radius -= _movingSphere._radius;


	//We create a new ray starting at the moving sphere's center with a direction of the
	//normalized moving sphere's speed
	Ray tempRay;
	tempRay._start = _movingSphere._center;
	tempRay._dir = glm::normalize(_sphereVel);

	//Now, we perform a modified ray to sphere test.
    
	//First, we create a vector from the static sphere's center to the ray's start point
	glm::vec3 vecV1 = tempRay._start - _staticSphere._center;

	//Get the dot product of this vector with the ray's direction
	//This will become the 'b' part of our quadratic equation
	float fB = glm::dot(vecV1, tempRay._dir);

	//Get the square distance from the start of the ray to the sphere's surface
	//This will become the 'c' part of our quadratic equation
	float fC = glm::dot(vecV1, vecV1) - (tempSphere._radius * tempSphere._radius);

	//We get the discriminant for our equation
	float fDisc = fB * fB - fC;

	//There's no need to check if the discriminant is negative. The reason is that,
	//since the ray starts inside the sphere, it will always collide with it
	//We solve our equation and get the time of collision
	//We use +sqrt(fDisc) to get the largest root, since the smaller root will be
	//an invalid value (negative time)
	_t = -fB + sqrt(fDisc);

	return true;
}
