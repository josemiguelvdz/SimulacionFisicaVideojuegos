#pragma once
#include "ForceGenerator.h"

class SpringForceGenerator : public ForceGenerator
{
public:
	SpringForceGenerator(double k, double restLength, Particle* other);

	virtual void updateForce(Particle* particle, double t) override;
	inline void setK(double k) {
		mK = k;
	}

	inline double getK() {
		return mK;
	}

protected:
	double mK; // Coef. elastico
	double mRestLength;
	Particle* mOther;
};

