#ifndef __PHYSICSINFO_H_
#define __PHYSICSINFO_H_
#include "AppClass.h"

namespace Simplex
{
	class PhysicsInfo
	{
	public:
		vector3 velocity;
		vector3 acceleration;
		float mass;
		const int MAX_SPEED = 0.5f;

		void ApplyForce(vector3 force);
		void UpdateVelocity();

		PhysicsInfo(float mss = 1.0f);
		~PhysicsInfo();
	};
} // end namespace Simplex
#endif //__PHYSICS_INFO_H_