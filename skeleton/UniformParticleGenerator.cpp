#include "UniformParticleGenerator.h"
#include <iostream>

UniformParticleGenerator::UniformParticleGenerator(physx::PxVec3 pos, physx::PxVec3 vel)
{
    d = std::uniform_real_distribution<double>{ 0, 1 }; // generate normal numbers

    mVel = vel;
    mPos = pos;
}

std::list<Particle*> UniformParticleGenerator::generateParticles()
{
    std::list<Particle*> lista;

    // std::cout << cont << std::endl;

    for (int i = 0; i < mMeanParticles && cont < pLimit; i++) {
        // todo: particles
        Particle* p = new Particle(mModel);
        p->setStaticParticle(false);

        p->setMass(1);
        p->setIMass(1);

        p->setPos(mPos);
        p->setColor(physx::PxVec4(0.2, 0.3, 1, 1));
        p->setVel(physx::PxVec3(d(generator) + mVel.x, d(generator) + mVel.y, d(generator) + mVel.z));

        lista.push_back(p);
        cont++;
    }
    return lista;
}
