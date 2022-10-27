#pragma once
#include "ParticleGenerator.h"

using namespace std;

class UniformParticleGenerator : public ParticleGenerator {
public:
	UniformParticleGenerator(physx::PxVec3 pos, physx::PxVec3 vel);
	virtual std::list<Particle*> generateParticles();

private:
	std::uniform_real_distribution<double> d;

	physx::PxVec3 mVel, mPos;

	random_device rand;
	default_random_engine generator = default_random_engine(rand());
};
