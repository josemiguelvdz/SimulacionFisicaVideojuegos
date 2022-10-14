#pragma once
#include <random>

#include "ParticleGenerator.h"

class ParticleSystem
{
public:
	ParticleSystem(int meanParticles, float varParts);

	void update(double t);
	ParticleGenerator* getParticleGenerator(std::string name);
	
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

};

