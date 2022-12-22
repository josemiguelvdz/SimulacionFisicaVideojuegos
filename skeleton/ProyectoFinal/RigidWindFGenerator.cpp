#include "RigidWindFGenerator.h"
#include <iostream>

RigidWindFGenerator::RigidWindFGenerator(const physx::PxVec3& windVel)
{
	mWindVel = windVel;
}

void RigidWindFGenerator::updateForce(PxRigidDynamic* particle, double t)
{
	if (particle != nullptr && mActive) {
		if (fabs(particle->getInvMass()) < 1e-10)
			return;

		particle->addForce(particle->mWindFriction * (mWindVel - particle->getLinearVelocity()) + particle->mWindFriction2 * windPow(mWindVel, 2));
	}
}
