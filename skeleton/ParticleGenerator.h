#pragma once
#include <list>
#include "Particle.h"

class ParticleGenerator {
public:
	virtual std::list<Particle*> generateParticles() = 0;

	inline void setOrigin(const vector3 p) { origin = p };
	inline void setMeanVelocity(const vector3 v) { setMeanVelocity}
	

protected:
	std::list<Particle*> mParticleCollection;

	// Origin
	physx::PxVec3 mOrigin;

	// Speed
	double mMeanSpeed;
	int mMeanParticles;
};