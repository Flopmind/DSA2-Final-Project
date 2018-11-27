#include "PhysicsInfo.h"
using namespace Simplex;


void Simplex::PhysicsInfo::ApplyForce(vector3 force)
{
	acceleration += force / mass;
}

void Simplex::PhysicsInfo::UpdateVelocity()
{
	if (acceleration == vector3(0.0f, 0.0f, 0.0f)) return;

	velocity += acceleration;

	// clamp magnitude of velocity to max speed
	if (glm::length(velocity) > MAX_SPEED)
	{
		velocity = glm::normalize(velocity) * MAX_SPEED;
	}
}

vector3 Simplex::PhysicsInfo::GetVelocity()
{
	return velocity;
}

PhysicsInfo::PhysicsInfo(float mss)
{
	velocity = vector3();
	acceleration = vector3();
	mass = mss;
}


PhysicsInfo::~PhysicsInfo()
{
}
