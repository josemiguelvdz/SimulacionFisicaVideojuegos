#pragma once
#include "PxPhysicsAPI.h"
#include "foundation/PxTransform.h"

#include "../Rigid/RigidForceGenerator.h"

#include <math.h>

using namespace physx;

class RigidTorbellinoFGenerator : public RigidForceGenerator
{
public:
	RigidTorbellinoFGenerator(const PxVec3& center, double rad, double force);
	virtual void updateForce(PxRigidDynamic* particle, double t);

	void invertTornado() { sentido *= -1; };
	
protected:
	PxVec3 mCenter;
	double mRad;
	double mForce;

	int mWindFriction = 1;
	int mWindFriction2 = 0.4;

	int sentido = 1;

	PxVec3 torbellinoPow(physx::PxVec3 v, double e) { return physx::PxVec3(pow(v.x, e), pow(v.y, e), pow(v.z, e)); };
	PxVec3 torbellinoVel(physx::PxVec3 pos);

	bool isInsideRange(PxRigidDynamic* p);
};

