#include "ParticleDragGenerator.h"
#include <iostream>

ParticleDragGenerator::ParticleDragGenerator()
{

}

ParticleDragGenerator::ParticleDragGenerator(const float k1, const float k2)
{
	mK1 = k1;
	mK2 = k2;
}

void ParticleDragGenerator::updateForce(Particle* particle, double t)
{
	if (fabs(particle->getIMass() < 1e-10))
		return;

	physx::PxVec3 v = particle->getVel();
	float drag_coef = v.magnitude();

	physx::PxVec3 dragF;
	drag_coef = mK1 * drag_coef + mK2 * drag_coef * drag_coef;

	dragF = -v * drag_coef;

	particle->addForce(dragF);
}
