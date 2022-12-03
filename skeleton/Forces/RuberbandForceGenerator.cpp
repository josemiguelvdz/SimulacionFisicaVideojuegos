#include "RuberbandForceGenerator.h"
#include <cmath>


RuberbandForceGenerator::RuberbandForceGenerator(double k, double restLength, Particle* other) : SpringForceGenerator(k, restLength, other)
{
	mK = k;
	mRestLength = restLength;
	mOther = other;
}

void RuberbandForceGenerator::updateForce(Particle* particle, double t)
{
	if (pow(mRestLength, 2) < (mOther->getPos() - particle->getPos()).magnitudeSquared()) { // formula de la distancia
		SpringForceGenerator::updateForce(particle, t);
	}
}
