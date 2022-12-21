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

	void setName(std::string newName) { mName = newName; };
	std::string getName() { return mName; };

	virtual physx::PxVec3 getStdPos() = 0;

	void subCont() { cont--; }

protected:
	// N_Particles
	int mMeanParticles;

	// string name
	std::string mName = "DEFAULT";

	// Particle model
	Particle* mModel;

	// Limit
	int pLimit = 320;
	int cont = 0;
};