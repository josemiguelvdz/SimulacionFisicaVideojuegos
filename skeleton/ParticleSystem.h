#pragma once
#include <random>

class ParticleSystem
{
public:
	ParticleSystem(int meanParticles, float varParts);
private:

	// Creaci�n de particulas
	int mNumParticles;
	int mMeanParticles;
	float mVarParts;


};

