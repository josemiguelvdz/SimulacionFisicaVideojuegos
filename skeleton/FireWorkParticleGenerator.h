#pragma once
#include <list>
#include "Particle.h"
#include <string>
#include <random>

using namespace std;

class FireWorkParticleGenerator {
public:
	FireWorkParticleGenerator();
	FireWorkParticleGenerator(physx::PxVec3 stdDevPos, physx::PxVec3 stdDevVel);
	~FireWorkParticleGenerator();

	//void createFireWorkParticle(Particle* p);

	//void integrate(double t);

	//void explode(Particle* parent);

	std::list<Particle*> planetExplosion(physx::PxVec3& iniPos, physx::PxVec3& iniVel, float& iniScale);

	// std::list<Particle*> getParticles() { return mParticles; };

protected:

	physx::PxVec3 pStdDevPos, pStdDevVel;

	normal_distribution<double> vel_x; // generate normal numbers // axis x
	normal_distribution<double> vel_y; // generate normal numbers // axis y
	normal_distribution<double> vel_z; // generate normal numbers // axis z

	random_device rand;
	default_random_engine generator = default_random_engine(rand());

	std::list<Particle*> mParticles;

	int numPartPerExplosion = 0;
};
