#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(int meanParticles, float varParts)
{
	mMeanParticles = meanParticles;
	mVarParts = varParts;


	mNumParticles = mMeanParticles + rand() * mVarParts;
}
