#pragma once
#include "ForceGenerator.h"
#include <math.h>

class WindForceGenerator : public ForceGenerator
{
public:
	WindForceGenerator(const physx::PxVec3& windVel);

	virtual void updateForce(Particle* particle, double t) override;

	physx::PxVec3 windPow(physx::PxVec3 v, double e) { return physx::PxVec3(pow(v.x, e), pow(v.y, e), pow(v.z, e)); };
	
protected:
	physx::PxVec3 mWindVel;
};

