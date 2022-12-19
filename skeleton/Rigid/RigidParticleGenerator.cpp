#include "RigidParticleGenerator.h"
#include "../Scene.h"
#include <cmath>

RigidParticleGenerator::RigidParticleGenerator(physx::PxVec3 stdDevPos, physx::PxVec3 stdDevVel, Scene* mSM)
{
    pStdDevPos = stdDevPos;
    pStdDevVel = stdDevVel;

    x = std::normal_distribution<double>{ 0, 4 }; // generate normal numbers // axis x
    y = std::normal_distribution<double>{ 0, 4 }; // generate normal numbers // axis y
    z = std::normal_distribution<double>{ 0, 4 }; // generate normal numbers // axis z

    mScene = mSM;
}

std::list<PxActor*> RigidParticleGenerator::generateParticles()
{
    std::list<PxActor*> listaRigids;
    listaRigids.clear();

    for (int i = 0; i < meanParticles; i++) {
        // todo: particles
        PxVec3 material = PxVec3(x(generator), y(generator), z(generator)).getNormalized();
        PxRigidDynamic* particle = mScene->createRigidDynamic(pStdDevPos, 
            mScene->getActivePhysics()->createMaterial(abs(material.x), abs(material.y), abs(material.z)), PxSphereGeometry(0.4), { 0, 0, 1, 1 }, INT_MAX);

        particle->setLinearVelocity(PxVec3(x(generator), pStdDevVel.y, z(generator)));

        particleCounter++;

        listaRigids.push_back(particle);
    }

    return listaRigids;
}
