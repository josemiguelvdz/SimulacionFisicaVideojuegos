#pragma once
#include <random>

#include "ParticleGenerator.h"

class ParticleSystem
{
public:
	ParticleSystem();
	~ParticleSystem();

	void Integrate(double t);
	ParticleGenerator* getParticleGenerator(std::string name);
	void addParticleGenerator(ParticleGenerator* pGenerator);

	
	// Fireworks
	// void generateFireworkSytem();

private:

	// Creación de particulas
	int mNumParticles;
	int mMeanParticles;
	float mVarParts;

	// Particles
	std::list<Particle*> mParticles;
	std::list<ParticleGenerator*> mParticleGenerators;

	bool RemoveParticle(int id);

};

