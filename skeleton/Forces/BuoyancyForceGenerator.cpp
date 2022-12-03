#include"BuoyancyForceGenerator.h"
#include "../RenderUtils.hpp"

BuoyancyForceGenerator::BuoyancyForceGenerator(Particle* liquid, float height, float density, float volume, float gravity)
{
	mLiquidSurface = liquid;
	mHeight = height;
	mDensity = density;
	mVolume = volume;
	mGravity = gravity;
}

void BuoyancyForceGenerator::updateForce(Particle* particle, double dt)
{
	float h0 = mLiquidSurface->getPos().y;
	float h = particle->getPos().y;

	float immersed;
	if (h - h0 > mHeight * .5)
		immersed = 0;
	else if (h0 - h > mHeight * .5)
		immersed = 1;
	else
		immersed = (h0 - h) / mHeight + .5;

	particle->addForce({ 0, mDensity * mVolume * immersed * mGravity, 0 });
}

void BuoyancyForceGenerator::setDensity(float newDensity)
{
	mDensity = newDensity;
	if (mDensity < .1)
		mDensity = .1;
}

void BuoyancyForceGenerator::setVolume(float newVolume)
{
	mVolume = newVolume;
}
