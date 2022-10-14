#pragma once
#include <list>
#include "Particle.h"
#include <string>

#include <random>

class ParticleGenerator {
public:
	virtual std::list<Particle*> generateParticles() = 0;

	inline void setOrigin(const physx::PxVec3 origin) { mOrigin = origin; };
	inline void setMeanVelocity(const physx::PxVec3 meanVel) { mMeanVelocity = meanVel; };
	inline void setMeanParticles(const int meanParticles) { mMeanParticles = meanParticles; };

	void setParticle(Particle* model);
	

protected:
	// Name
	std::string mName;

	// Particle collection
	std::list<Particle*> mParticleCollection;

	// Mean speed & position
	physx::PxVec3 mMeanVelocity;
	physx::PxVec3 mOrigin;

	// N_Particles
	int mMeanParticles;

	// Particle model
	Particle* mModel;

	// Generation probability
	double mGenProb;
};