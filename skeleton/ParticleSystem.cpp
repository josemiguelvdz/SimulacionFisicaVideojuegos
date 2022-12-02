#include "ParticleSystem.h"


#include <iostream>

ParticleSystem::ParticleSystem()
{
	// basic
	gForceGenerator = new GravityForceGenerator(physx::PxVec3(0, -30, 0));
	// dForceGenerator = new ParticleDragGenerator(0.03, 0);
	// wForceGenerator = new WindForceGenerator(physx::PxVec3(-10, 0, 0));

	// torbellino y explosion
	//tForceGenerator = new TorbellinoForceGenerator(physx::PxVec3(30, -20, 30), 5, 20);
	// eForceGenerator = new ExplosionForceGenerator(physx::PxVec3(30, 50, 30), 50, 70, 0, 100, 2000);
}

ParticleSystem::~ParticleSystem() {
	for (auto p : mParticles)
		delete p;

	for (auto g : mParticleGenerators)
		delete g;
}

void ParticleSystem::Integrate(double t)
{
	static int contador = 0;
	for (auto g : mParticleGenerators) {
		std::list<Particle*> test = g->generateParticles();
		for (auto i : test) {
			mParticles.push_back(i);

			// contador++;
			// std::cout << "contador: " << contador << "\n";
		}
			
	}
	
	std::list<Particle*>::iterator it = mParticles.begin();

	while (it != mParticles.end())
	{
		if (!(*it)->isAlive()) {
			delete *it;
			it = mParticles.erase(it);
		}
		else {
			if (gForceGenerator != nullptr)
				gForceGenerator->updateForce(*it, t);

			if (dForceGenerator != nullptr)
				dForceGenerator->updateForce(*it, t);

			if (wForceGenerator != nullptr)
				wForceGenerator->updateForce(*it, t);

			if (tForceGenerator != nullptr)
				tForceGenerator->updateForce(*it, t);

			if (eForceGenerator != nullptr)
				eForceGenerator->updateForce(*it, t);

			(*it)->integrate(t);

			it++;
		}
			
	}
}

void ParticleSystem::addParticleGenerator(ParticleGenerator* pGenerator)
{
	mParticleGenerators.push_back(pGenerator);
}
