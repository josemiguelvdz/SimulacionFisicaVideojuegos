#pragma once
using namespace std;

#include "../Particle.h"

class ForceGenerator {
public:
	virtual void updateForce(Particle* particle, double duration) = 0;
	std::string mName;
	double t = -1e10;

	bool mActive = true;

	void setActive(bool newActive) { mActive = newActive; };
};
