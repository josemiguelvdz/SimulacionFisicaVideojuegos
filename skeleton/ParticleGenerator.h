#pragma once
#include <list>
#include "Particle.h"
#include <string>
#include <random>

class ParticleGenerator {
public:
	virtual std::list<Particle*> generateParticles() = 0;

	void setParticle(Particle* model);

	void setMeanParticles(int newMeanParticles) { mMeanParticles = newMeanParticles; };

protected:
	// N_Particles
	int mMeanParticles;

	// Particle model
	Particle* mModel;
};