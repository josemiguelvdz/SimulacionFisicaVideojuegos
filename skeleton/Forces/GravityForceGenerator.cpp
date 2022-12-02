#include "GravityForceGenerator.h"

GravityForceGenerator::GravityForceGenerator(const physx::PxVec3& g)
{
	mGravity = g;
}

void GravityForceGenerator::updateForce(Particle* particle, double t)
{
	if (particle != nullptr && mActive) {
		if (fabs(particle->getIMass()) < 1e-10)
			return;

		particle->addForce(mGravity * particle->getMass());
	}
}
