#include "PhysicsInfo.h"
using namespace Simplex;


void Simplex::PhysicsInfo::ApplyForce(vector3 force)
{
	acceleration += force / mass;
}

void Simplex::PhysicsInfo::UpdateVelocity()
{
	velocity += acceleration;

	if (acceleration == vector3(0.0f) && velocity.length() < frictionMagnitude)
	{
		velocity = vector3(0.0f);
	}
	else if (acceleration == vector3(0.0f) && velocity.length() > frictionMagnitude)
	{
		velocity += frictionMagnitude * -1 * glm::normalize(velocity);
	}

	// clamp magnitude of velocity to max speed
	if (glm::length(velocity) > MAX_SPEED)
	{
		velocity = glm::normalize(velocity) * MAX_SPEED;
	}
}


void Simplex::PhysicsInfo::Collision(PhysicsInfo info)
{
	vector3 oldVel = velocity;
	vector3 nextVelDirect = glm::normalize(info.position - position);


	//make all ball to ball collisions elastic

	float angle = glm::dot(oldVel, nextVelDirect) / (oldVel.length() * nextVelDirect.length());
	angle = sin(angle);
	if (angle < 0)
		angle *= -1;
	if (info.velocity.length() == 0 && angle != 1)
	{
		//This next line is not correct, use if actual line isn't working and you absolutely need something
		//info.velocity = 0.5f * oldVel.length * nextVelDirect;

		//actual line
		info.velocity = angle * oldVel.length() * nextVelDirect;
		velocity = sqrt((oldVel.length() * oldVel) - (info.velocity.length() * info.velocity));
	}
	else if (info.velocity.length() == 0)
	{
		info.velocity = oldVel;
		velocity = vector3(0.0f);
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
