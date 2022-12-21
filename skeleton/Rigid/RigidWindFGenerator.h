#pragma once
#include "PxPhysicsAPI.h"
#include "foundation/PxTransform.h"

#include "RigidForceGenerator.h"

#include <math.h>

using namespace physx;

class RigidWindFGenerator : public RigidForceGenerator
{
public:
	RigidWindFGenerator(const physx::PxVec3& windVel);
	virtual void updateForce(PxRigidDynamic* particle, double t);
	
	physx::PxVec3 windPow(physx::PxVec3 v, double e) { return physx::PxVec3(pow(v.x, e), pow(v.y, e), pow(v.z, e)); };

protected:
	physx::PxVec3 mWindVel;
};

