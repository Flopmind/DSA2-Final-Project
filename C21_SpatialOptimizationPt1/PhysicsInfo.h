#ifndef __PHYSICSINFO_H_
#define __PHYSICSINFO_H_

#include "Definitions.h"


namespace Simplex
{
	class PhysicsInfo
	{
	public:
		vector3 velocity;
		vector3 acceleration;
		float mass;
		const float MAX_SPEED = 1.0f;

		void ApplyForce(vector3 force);
		void UpdateVelocity();
		vector3 GetVelocity();

		PhysicsInfo(float mss = 1.0f);
		~PhysicsInfo();
	};
} // end namespace Simplex
#endif //__PHYSICS_INFO_H_