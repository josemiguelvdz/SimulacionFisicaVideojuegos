#pragma once
#include "ParticleGenerator.h"

class GaussianParticleGenerator : public ParticleGenerator {
public:
	GaussianParticleGenerator(physx::PxVec3 stdDevPos, physx::PxVec3 physx stdDevVel);
	virtual std::list<Particle*> generateParticles();
protected:
	physx::PxVec3 pStdDevPos, pStdDevVel;
};
