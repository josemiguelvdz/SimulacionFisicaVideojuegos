#pragma once

// Forces
#include "./GravityForceGenerator.h"
#include "./ParticleDragGenerator.h"
#include "./WindForceGenerator.h"
#include "./TorbellinoForceGenerator.h"
#include "./ExplosionForceGenerator.h"
#include "./SpringForceGenerator.h"
#include "./SlinkyForceGenerator.h"


#include <unordered_map>;
#include "ForceGenerator.h"
#include "../Particle.h"

#include <climits>

using namespace std;

typedef unordered_multimap<ForceGenerator*, Particle*> mapFR;

class ForceRegistry : public mapFR
{
public:
	void Integrate(double dt);
	mapFR::iterator AddRegistry(ForceGenerator* fg, Particle* p);

	void DeleteParticle(Particle* p);
	void DeleteForce(ForceGenerator* fg);
};

