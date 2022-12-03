#pragma once
#include "ForceGenerator.h"

class BuoyancyForceGenerator : public ForceGenerator
{
protected:
	float mDensity, mGravity, mHeight, mVolume;
	Particle* mLiquidSurface = nullptr;
public:
	BuoyancyForceGenerator(Particle* liquid, float height, float density, float volume, float gravity = 9.8);

	virtual void updateForce(Particle* particle, double t) override;

	void setDensity(float newDensity);
	float getDensity() { return mDensity; };
	void setVolume(float newVolume);
	float getVolume() { return mVolume; };
};

