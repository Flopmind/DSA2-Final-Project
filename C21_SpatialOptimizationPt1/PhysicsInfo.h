#ifndef __PHYSICSINFO_H_
#define __PHYSICSINFO_H_

#include "Definitions.h"


namespace Simplex
{
	class PhysicsInfo
	{
	vector3 normalize(const vector3 &v);
	public:
		vector3 position;
		vector3 velocity;
		vector3 acceleration;
		vector3 center;
		vector3 limits;
		float mass;
		const float MAX_SPEED = 1.0f;
		float frictionMagnitude;

		void ApplyForce(vector3 force);
		void UpdateVelocity();
		void Collision(PhysicsInfo info);
		vector3 GetVelocity();

		PhysicsInfo(float mss = 1.0f, vector3 pos = vector3(0.0f), vector3 center = vector3(0.0f), vector3 limits = vector3(50.0f));
		~PhysicsInfo();
	};
} // end namespace Simplex
#endif //__PHYSICS_INFO_H_