#include "PhysicsInfo.h"
using namespace Simplex;


void Simplex::PhysicsInfo::ApplyForce(vector3 force)
{
	acceleration += force / mass;
}

void Simplex::PhysicsInfo::UpdateVelocity()
{
	velocity += acceleration;
	acceleration = vector3(0.0f);
	if (center.x + position.x > limits.x || center.x + position.x < -limits.x)
	{
		velocity.x *= -1;
	}
	if (center.y + position.y > limits.y || center.y + position.y < -limits.y)
	{
		velocity.y *= -1;
	}
	if (center.z + position.z > limits.z || center.z + position.z < -limits.z)
	{
		velocity.z *= -1;
	}
	float velMag = magnitude(velocity);
	if (velMag > 0)
	{
		vector3 frictionForce = (-frictionMagnitude * glm::normalize(velocity));
		//std::cout << "fric - " << magnitude(frictionForce) << ";" << std::endl;
		//std::cout << "vel - " << magnitude(velocity) << ";" << std::endl;
		//std::cout << "net - " << magnitude(velocity - frictionForce) << ";" << std::endl;
		//std::cout << "net2 - " << magnitude(velocity)  - magnitude(frictionForce) << ";" << std::endl;
		if (frictionMagnitude > velMag)
		{
			velocity = vector3(0.0f);
		}
		else
		{
			velocity += frictionForce;
		}
	}
	// clamp magnitude of velocity to max speed
	if (magnitude(velocity) > MAX_SPEED)
	{
		velocity = glm::normalize(velocity) * MAX_SPEED;
	}
	position += velocity;
}


void Simplex::PhysicsInfo::Collision(PhysicsInfo info)
{
	vector3 oldVel = velocity;
	if (magnitude(oldVel) == 0.0f)
	{
		return;
	}
	vector3 nextVelDirect = glm::normalize(info.position - position);


	//make all ball to ball collisions elastic
	float getAngle = glm::dot(glm::normalize(oldVel), nextVelDirect);
	// [-1, 1]
	std::cout << "angle - " << getAngle << std::endl;
	float angle = acosf(getAngle);
	angle = sinf(angle);
	if (angle < 0)
		angle *= -1;
	float nextVecMag;
	if (magnitude(info.velocity) == 0 && angle != 1)
	{
		//This next line is not correct, use if actual line isn't working and you absolutely need something
		//info.velocity = 0.5f * oldVel.length * nextVelDirect;

		//actual line
		info.SetVelocity(angle * magnitude(oldVel) * nextVelDirect);
		vector3 nextVec = (magnitude(oldVel) * oldVel) - (magnitude(info.velocity) * info.velocity);
		nextVecMag = magnitude(nextVec);
		if (nextVecMag < 0)
		{
			nextVecMag *= -1;
		}
		nextVecMag = sqrt(nextVecMag);
		velocity = nextVecMag * glm::normalize(nextVec);
	}
	else if (magnitude(info.velocity) == 0)
	{
		info.SetVelocity(oldVel);
		velocity = vector3(0.0f);
	}
	if (isnan(velocity.x) || isnan(velocity.y) || isnan(velocity.z))
	{
		std::cout << "-" << std::endl;
	}
}

vector3 Simplex::PhysicsInfo::GetVelocity()
{
	return velocity;
}

PhysicsInfo::PhysicsInfo(float mss, vector3 pos, vector3 cent, vector3 limit)
{
	velocity = vector3(0.1f);
	acceleration = vector3(0.0f);
	mass = mss;
	position = pos;
	center = cent;
	limits = limit;
	frictionMagnitude = 0.005f;
}


PhysicsInfo::~PhysicsInfo()
{
}

vector3 PhysicsInfo::normalize(const vector3 &v)
{
	float sum = (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
	if (sum < 0)
	{
		sum *= -1;
	}
	float length_of_v = sqrt(sum);
	return vector3(v.x / length_of_v, v.y / length_of_v, v.z / length_of_v);
}

float PhysicsInfo::magnitude(const vector3 &v)
{
	float sum = (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
	if (sum < 0)
	{
		sum *= -1;
	}
	float length_of_v = sqrt(sum);
	if (isnan(length_of_v))
	{
		throw ExceptionCollidedUnwind;
	}
	return length_of_v;
}

float PhysicsInfo::GetMagnitude()
{
	return magnitude(velocity);
}

void PhysicsInfo::SetVelocity(vector3 nextVel)
{
	velocity = nextVel;
}