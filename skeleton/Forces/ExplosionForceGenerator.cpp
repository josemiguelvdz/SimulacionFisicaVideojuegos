#include "ExplosionForceGenerator.h"
#include <math.h>
#include <iostream>

ExplosionForceGenerator::ExplosionForceGenerator(physx::PxVec3 center, double rad, double maxRad, double time, double dur, double force)
{
	mCenter = center;
	mRad = rad;
	mMaxRad = maxRad;
	mTime = time;
	mDur = dur;
	mForce = force;
}

void ExplosionForceGenerator::updateForce(Particle* particle, double t)
{
	mTime += t;
	mRad = mMaxRad * mTime / mDur;

	//cout << mTime << "\n";

	if (particle != nullptr && mTime > mExpTime) {
		if (fabs(particle->getIMass()) < 1e-10)
			return;
		
		if ((particle->getPos() - mCenter).magnitudeSquared() < pow(mRad, 2)) {
			double distance = expDist(particle, mCenter);
			particle->addForce(mForce / pow(distance, 2) * (particle->getPos() - mCenter) * exp(-mTime / mDur));
		}
	}
}

double ExplosionForceGenerator::expDist(Particle* particle, physx::PxVec3 expPos) {
	return sqrt(pow(particle->getPos().x - expPos.x, 2) + pow(particle->getPos().y - expPos.y, 2) + pow(particle->getPos().z - expPos.z, 2));
}
