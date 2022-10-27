#include "GaussianParticleGenerator.h"

GaussianParticleGenerator::GaussianParticleGenerator(physx::PxVec3 stdDevPos, physx::PxVec3 stdDevVel, double mean, double desviation)
{
    pStdDevPos = stdDevPos;
    pStdDevVel = stdDevVel;

    y = std::normal_distribution<double>{mean, desviation}; // generate normal numbers

    x = std::normal_distribution<double>{0, 4}; // generate normal numbers // axis x
}

std::list<Particle*> GaussianParticleGenerator::generateParticles()
{
    std::list<Particle*> lista;

    for (int i = 0; i < mMeanParticles; i++) {
        // todo: particles
        Particle* p = new Particle(mModel);
        p->setPos(pStdDevPos);
        p->setColor(physx::PxVec4(y(generator), y(generator), y(generator), 1));

        p->setVel(physx::PxVec3(x(generator), y(generator) + pStdDevVel.y, 0));

        lista.push_back(p);
    }
    return lista;
}
