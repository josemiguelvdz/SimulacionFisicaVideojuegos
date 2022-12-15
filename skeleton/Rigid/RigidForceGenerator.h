#pragma once
#include "PxPhysicsAPI.h"
#include <math.h>

using namespace physx;

class RigidForceGenerator
{
public:
	virtual void updateForce(PxRigidDynamic* particle, double t) = 0;
	
	double t = -1e10;
	bool mActive;

	void setActive(bool newActive) { mActive = newActive; };
};

