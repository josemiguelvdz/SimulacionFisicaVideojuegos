#include "GravityForceGenerator.h"
#include <iostream>

GravityForceGenerator::GravityForceGenerator(const physx::PxVec3& g)
{
	mGravity = g;
}

void GravityForceGenerator::updateForce(Particle* particle, double t)
{
	if (particle != nullptr && mActive) {
		if (fabs(particle->getIMass()) < 1e-10)
			return;

		physx::PxVec3 force = { mGravity * particle->getMass() };
		/*std::cout << "gravity : " << force.x << " " << force.y << " " << force.z << std::endl;*/
		particle->addForce(force);
	}
}
