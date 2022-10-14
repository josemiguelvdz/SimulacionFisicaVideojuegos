#include "UniformParticleGenerator.h"

UniformParticleGenerator::UniformParticleGenerator(physx::PxVec3 pos, physx::PxVec3 vel, double iMass)
{

}

std::list<Particle*> UniformParticleGenerator::generateParticles()
{
    for (int i = 0; i < mMeanParticles; i++) {
        // todo: particles
    }
    return std::list<Particle*>();
}
