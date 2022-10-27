#include "ParticleSystem.h"

#include <iostream>

ParticleSystem::ParticleSystem()
{
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
			(*it)->integrate(t);
			it++;
		}
			
	}
}

void ParticleSystem::addParticleGenerator(ParticleGenerator* pGenerator)
{
	mParticleGenerators.push_back(pGenerator);
}
