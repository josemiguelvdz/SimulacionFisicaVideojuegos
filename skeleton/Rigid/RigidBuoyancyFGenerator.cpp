#include "RigidBuoyancyFGenerator.h"
#include "../Particle.h"
#include <iostream>

RigidBuoyancyFGenerator::RigidBuoyancyFGenerator(Particle* liquid, float height, float density, float volume, float gravity)
{
	mLiquidSurface = liquid;
	mHeight = height;
	mDensity = density;
	mVolume = volume;
	mGravity = gravity;
}

void RigidBuoyancyFGenerator::updateForce(PxRigidDynamic* particle, double t)
{
	if (particle != nullptr && mActive) {
		if (fabs(particle->getInvMass()) < 1e-10)
			return;

		float h0 = mLiquidSurface->getPos().y;
		float h = particle->getGlobalPose().p.y;

		float immersed;
		if (h - h0 > mHeight * .5)
			immersed = 0;
		else if (h0 - h > mHeight * .5)
			immersed = 1;
		else
			immersed = (h0 - h) / mHeight + .5;

		PxVec3 force = { 0, mDensity * mVolume * immersed * mGravity, 0 };
	
		particle->addForce(force);
	}
}

void RigidBuoyancyFGenerator::setDensity(float newDensity)
{
	mDensity = newDensity;
	if (mDensity < .1)
		mDensity = .1;
}

void RigidBuoyancyFGenerator::setVolume(float newVolume)
{
	mVolume = newVolume;
}
