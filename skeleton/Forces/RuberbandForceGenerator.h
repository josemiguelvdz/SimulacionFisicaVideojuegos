#pragma once
#include "SpringForceGenerator.h"

class RuberbandForceGenerator : public SpringForceGenerator
{
public:
	RuberbandForceGenerator(double k, double restLength, Particle* other);

	void updateForce(Particle* particle, double t) override;

protected:
	double mK; // Coef. elastico
	double mRestLength;
	Particle* mOther;
};

