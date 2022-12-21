#pragma once
#include "ForceGenerator.h"
#include <math.h>

class TorbellinoForceGenerator : public ForceGenerator
{
public:
	TorbellinoForceGenerator(const physx::PxVec3& center, double rad, double force);

	virtual void updateForce(Particle* particle, double t) override;
	
protected:
	physx::PxVec3 mCenter;
	double mRad;
	double mForce;

	bool isInsideRange(Particle* p);

	physx::PxVec3 torbellinoPow(physx::PxVec3 v, double e) { return physx::PxVec3(pow(v.x, e), pow(v.y, e), pow(v.z, e)); };
	physx::PxVec3 torbellinoVel(physx::PxVec3 pos, physx::PxVec3 pVel, double t);
};

