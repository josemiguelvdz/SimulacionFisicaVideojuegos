#pragma once
#include <random>

#include "ParticleGenerator.h"

#include "./FireWorkParticleGenerator.h"
#include "./Forces/ForceRegistry.h"

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void Integrate(double t);
	ParticleGenerator* getParticleGenerator(std::string name);
	void addParticleGenerator(ParticleGenerator* pGenerator);

	ForceRegistry* getForceRegistry() { return fRegistry; };

	void addParticle(Particle* p) { mParticles.push_back(p); };


	// Fireworks
	void planetExplosion(physx::PxVec3& iniPos, physx::PxVec3& iniVel, float& iniScale);

	void ParticleSystem::InitTornados();

	// Test
	void addCubeToTornado(Particle* c) {
		c->setAffectedByTornado(true);

		for(auto t : mTornados)
			fRegistry->AddRegistry(t, c);
	}

private:
	// Particles
	std::list<Particle*> mParticles;
	std::list<ParticleGenerator*> mParticleGenerators;

	// List that associates particles with generators
	std::list<std::pair<Particle*, ParticleGenerator*>> mTornadoParticles;

	FireWorkParticleGenerator* fw = nullptr;

	std::list<TorbellinoForceGenerator*> mTornados;
	//TorbellinoForceGenerator* tForce = nullptr;

	ForceRegistry* fRegistry = nullptr;
	

};

