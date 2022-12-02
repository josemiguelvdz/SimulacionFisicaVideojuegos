#include "WindForceGenerator.h"


WindForceGenerator::WindForceGenerator(const physx::PxVec3& windVel)
{
	mWindVel = windVel;
}

void WindForceGenerator::updateForce(Particle* particle, double t)
{
	if (particle != nullptr && mActive) {
		if (fabs(particle->getIMass()) < 1e-10)
			return;

		particle->addForce(particle->getWindFriction() * (mWindVel - particle->getVel()) + particle->getWindFriction2() * windPow(mWindVel, 2));
	}
}
