#include "TorbellinoForceGenerator.h"


TorbellinoForceGenerator::TorbellinoForceGenerator(const physx::PxVec3& center, double rad, double force)
{
	mCenter = center;
	mRad = rad;
	mForce = force;
}

void TorbellinoForceGenerator::updateForce(Particle* particle, double t)
{
	if (particle != nullptr) {
		if (fabs(particle->getIMass()) < 1e-10)
			return;

		particle->addForce(particle->getWindFriction() * mForce * torbellinoVel(particle->getPos()) +
			particle->getWindFriction2() * torbellinoPow(mForce * torbellinoVel(particle->getPos()), 2));
	}
}

physx::PxVec3 TorbellinoForceGenerator::torbellinoVel(physx::PxVec3 pos) {

	physx::PxVec3 vel = pos - mCenter;
	vel = vel.cross(physx::PxVec3(0, 1, 0)) + 5 * -vel;
	vel.y = 0;
	return vel;
}
