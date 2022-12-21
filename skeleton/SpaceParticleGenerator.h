#pragma once
#include "ParticleGenerator.h"

using namespace std;

class SpaceParticleGenerator : public ParticleGenerator {
public:
	SpaceParticleGenerator(physx::PxVec3 stdDevPos, physx::PxVec3 stdDevVel);
	virtual std::list<Particle*> generateParticles() override;

	virtual physx::PxVec3 getStdPos() { return pStdDevPos; };

protected:
	physx::PxVec3 pStdDevPos, pStdDevVel;

	normal_distribution<double> x; // generate normal numbers // axis x
	normal_distribution<double> y; // generate normal numbers // axis y
	normal_distribution<double> z; // generate normal numbers // axis z

	normal_distribution<double> pos; // generate normal numbers // pos

	random_device rand;
	default_random_engine generator = default_random_engine(rand());
};
