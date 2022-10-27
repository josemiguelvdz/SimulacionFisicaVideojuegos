#pragma once
#include "ParticleGenerator.h"

using namespace std;

class NieveParticleGenerator : public ParticleGenerator {
public:
	NieveParticleGenerator(physx::PxVec3 stdDevPos, physx::PxVec3 stdDevVel, double media, double desviacion);
	virtual std::list<Particle*> generateParticles() override;

protected:
	physx::PxVec3 pStdDevPos, pStdDevVel;

	normal_distribution<double> y; // generate normal numbers // axis y
	normal_distribution<double> x; // generate normal numbers // axis x

	normal_distribution<double> pos; // generate normal numbers // axis x

	random_device rand;
	default_random_engine generator = default_random_engine(rand());
};
