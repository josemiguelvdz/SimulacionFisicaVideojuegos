#pragma once
#include "ParticleGenerator.h"

class UniformParticleGenerator : ParticleGenerator {
public:
	UniformParticleGenerator(physx::PxVec3 pos, physx::PxVec3 vel, double iMass);
	virtual std::list<Particle*> generateParticles();
};
