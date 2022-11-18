#pragma once
#include "ForceGenerator.h"
class ParticleDragGenerator : public ForceGenerator
{
public:
	ParticleDragGenerator();
	ParticleDragGenerator(const float k1, const float k2);

	virtual void updateForce(Particle* particle, double t);

	inline void setDrag(float k1, float k2) { mK1 = k1; mK2 = k2; };

	inline float getK1() { return mK1; };
	inline float getK2() { return mK2; };
protected:
	float mK1; // Coef for velocity
	float mK2; // Coef for square velocity
};

