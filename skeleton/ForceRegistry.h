#pragma once
#include <map>
#include "ForceGenerator.h"

using namespace std;

typedef std::pair<ForceGenerator*, Particle*> FRPair;

class ForceRegistry : multimap<ForceGenerator*, Particle*> {
public:
	void updateForces(double duration) {
		for (auto it = begin(); it != end(); it++)
			it->first->updateForce(it->second, duration);
	}
	
	void addRegistry(ForceGenerator* fg, Particle* p) {
		FRPair pair;
		pair.first = fg;
		pair.second = p;
		insert(pair);
	}

	void deleteParticleRegistry(Particle* p) {
		for (auto it = begin(); it != end(); it++) {
			if (it->second == p)
				delete p;
		}
	}
};
