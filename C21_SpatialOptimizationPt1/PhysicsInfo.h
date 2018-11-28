#ifndef __PHYSICSINFO_H_
#define __PHYSICSINFO_H_

#include "Definitions.h"


namespace Simplex
{
	class PhysicsInfo
	{
	public:
		vector3 position;
		vector3 velocity;
		vector3 acceleration;
		float mass;
		const float MAX_SPEED = 1.0f;
		float frictionMagnitude;

		void ApplyForce(vector3 force);
		void UpdateVelocity();
		void Collision(PhysicsInfo info);
		vector3 GetVelocity();

		PhysicsInfo(float mss = 1.0f);
		~PhysicsInfo();
	};
} // end namespace Simplex
#endif //__PHYSICS_INFO_H_