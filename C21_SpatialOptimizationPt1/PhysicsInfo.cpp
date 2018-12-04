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
	if (velocity.length() > 0)
	{
		vector3 frictionForce = (-1 * normalize(velocity));
		std::cout << "fric - " << (frictionForce).length() << ";" << std::endl;
		std::cout << "vel - " << (velocity).length() << ";" << std::endl;
		std::cout << "net - " << (velocity - frictionForce).length() << ";" << std::endl;
		std::cout << "net2 - " << (velocity.length())  - (frictionForce).length() << ";" << std::endl;
		if (frictionForce.length() > velocity.length())
		{
			velocity = vector3(0.0f);
		}
		else
		{
			velocity -= frictionForce;
			//std::cout << frictionForce.length() << ";" << std::endl;
		}
	}
	// clamp magnitude of velocity to max speed
	if (glm::length(velocity) > MAX_SPEED)
	{
		velocity = glm::normalize(velocity) * MAX_SPEED;
	}
	position += velocity;
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

PhysicsInfo::PhysicsInfo(float mss, vector3 pos, vector3 cent, vector3 limit)
{
	velocity = vector3(1);
	acceleration = vector3(0.0f);
	mass = mss;
	position = pos;
	center = cent;
	limits = limit;
	frictionMagnitude = 2;
}


PhysicsInfo::~PhysicsInfo()
{
}

vector3 PhysicsInfo::normalize(const vector3 &v)
{
	float length_of_v = sqrt((v.x * v.x) + (v.y * v.y) + (v.z * v.z));
	return vector3(v.x / length_of_v, v.y / length_of_v, v.z / length_of_v);
}