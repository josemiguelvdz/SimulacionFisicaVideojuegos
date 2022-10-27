#include "SpaceParticleGenerator.h"

SpaceParticleGenerator::SpaceParticleGenerator(physx::PxVec3 stdDevPos, physx::PxVec3 stdDevVel)
{
    pStdDevPos = stdDevPos;
    pStdDevVel = stdDevVel;

    x = std::normal_distribution<double>{0, .1}; // generate normal numbers // axis x
    y = std::normal_distribution<double>{0, .1}; // generate normal numbers // axis y
    z = std::normal_distribution<double>{0, .1}; // generate normal numbers // axis z


    pos = std::normal_distribution<double>{0, 500}; // generate normal numbers // axis x
}

std::list<Particle*> SpaceParticleGenerator::generateParticles()
{
    std::list<Particle*> lista;

    for (int i = 0; i < mMeanParticles; i++) {
        // todo: particles
        if (rand() % 2 == 0) {
            Particle* p = new Particle(mModel);
            p->setPos(physx::PxVec3(pos(generator) + pStdDevPos.x, pos(generator) + pStdDevPos.y, pos(generator) + pStdDevPos.z));

            p->setVel(physx::PxVec3(x(generator) + pStdDevVel.x, y(generator) + pStdDevVel.y, x(generator) + pStdDevVel.z));

            lista.push_back(p);
        }
        
    }
    return lista;
}