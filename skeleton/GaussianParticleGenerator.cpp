#include "GaussianParticleGenerator.h"

GaussianParticleGenerator::GaussianParticleGenerator(physx::PxVec3 stdDevPos, physx::PxVec3 stdDevVel)
{
}

std::list<Particle*> GaussianParticleGenerator::generateParticles()
{
    for (int i = 0; i < mMeanParticles; i++) {
        // todo: particles
    }
    return std::list<Particle*>();
}
