#include "SpringForceGenerator.h"
#include <iostream>

SpringForceGenerator::SpringForceGenerator(double k, double restLength, Particle* other)
{
	mK = k;
	mRestLength = restLength;
	mOther = other;
}

void SpringForceGenerator::updateForce(Particle* particle, double t)
{
	if (particle != nullptr && mOther != nullptr && mOther != particle) {
		if (fabs(particle->getIMass()) < 1e-10)
			return;

		physx::PxVec3 force = mOther->getPos() - particle->getPos();

		

		const float length = force.normalize();
		const float deltaX = length - mRestLength;

		force *= deltaX * mK;

		particle->addForce(force);
	}
}
