#pragma once
#include "PxPhysicsAPI.h"
#include "foundation/PxTransform.h"

#include <list>
#include <random>

using namespace physx;

class Scene;

class RigidParticleGenerator
{
public:
	RigidParticleGenerator(physx::PxVec3 stdDevPos, physx::PxVec3 stdDevVel, Scene* mSM);
	std::list<PxActor*> generateParticles();

	int getParticleCounter() { return particleCounter; };

protected:
	Scene* mScene = nullptr;

	PxVec3 pStdDevPos, pStdDevVel;

	std::normal_distribution<double> x; // generate normal numbers // axis x
	std::normal_distribution<double> y; // generate normal numbers // axis y
	std::normal_distribution<double> z; // generate normal numbers // axis z

	std::random_device rand;
	std::default_random_engine generator = std::default_random_engine(std::rand());

	int meanParticles = 1;
	int particleCounter = 0;
};

