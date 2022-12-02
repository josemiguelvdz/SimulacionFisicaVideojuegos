#include "ExplosionForceGenerator.h"
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

	if (mTime > mExpTime) {
		canExplode = !canExplode;
		mTime = 0;
	}

	cout << mTime << "\n";

	if (particle != nullptr && canExplode && mActive) {
		if (fabs(particle->getIMass()) < 1e-10)
			return;
		
		if ((particle->getPos() - mCenter).magnitudeSquared() < pow(mRad, 2)) {
			double distance = expDist(particle, mCenter);

			physx::PxVec3 force = physx::PxVec3(mForce / pow(distance, 2));

			physx::PxVec3 centerVec = particle->getPos() - mCenter;

			physx::PxVec3 finalForce = physx::PxVec3(force.x * centerVec.x, force.y * centerVec.y, force.z * centerVec.z);

			particle->addForce(finalForce * exp(-mTime / mDur));
		}
	}
}

double ExplosionForceGenerator::expDist(Particle* particle, physx::PxVec3 expPos) {
	return sqrt(pow(particle->getPos().x - expPos.x, 2) + pow(particle->getPos().y - expPos.y, 2) + pow(particle->getPos().z - expPos.z, 2));
}
