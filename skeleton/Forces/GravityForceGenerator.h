#pragma once
#include "ForceGenerator.h"

class GravityForceGenerator : public ForceGenerator
{
public:
	GravityForceGenerator(const physx::PxVec3& g);

	virtual void updateForce(Particle* particle, double t) override;
	inline void setGravity(physx::PxVec3 g) {
		mGravity = g;
	}
protected:
	physx::PxVec3 mGravity;
};

