#pragma once
#include "ForceGenerator.h"
#include <cmath>

using namespace std;

class ExplosionForceGenerator : public ForceGenerator
{
public:
	ExplosionForceGenerator(physx::PxVec3 center, double rad, double maxRad, double time, double dur, double force);

	virtual void updateForce(Particle* particle, double t) override;
protected:
	physx::PxVec3 mCenter;
	double mRad;
	double mMaxRad;
	double mTime;
	double mDur;
	double mForce;

	double mExpTime = 500; // Delay Exp

	bool canExplode = false;

	physx::PxVec3 expPow(physx::PxVec3 v, double e) { return physx::PxVec3(pow(v.x, e), pow(v.y, e), pow(v.z, e)); };
	double expDist(Particle* particle, physx::PxVec3 pos);
};

