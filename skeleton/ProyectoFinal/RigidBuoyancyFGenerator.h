#pragma once
#include "PxPhysicsAPI.h"
#include "foundation/PxTransform.h"

#include "../Rigid/RigidForceGenerator.h"

#include <math.h>

using namespace physx;

class Particle;

class RigidBuoyancyFGenerator : public RigidForceGenerator
{
public:
	RigidBuoyancyFGenerator(Particle* liquid, float height, float density, float volume, float gravity = 9.8);
	virtual void updateForce(PxRigidDynamic* particle, double t);

	void setDensity(float newDensity);
	float getDensity() { return mDensity; };
	void setVolume(float newVolume);
	float getVolume() { return mVolume; };

protected:
	float mDensity, mGravity, mHeight, mVolume;
	Particle* mLiquidSurface = nullptr;
};

