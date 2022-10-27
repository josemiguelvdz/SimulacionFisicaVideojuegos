#include "FireWorkParticleGenerator.h"

FireWorkParticleGenerator::FireWorkParticleGenerator(physx::PxVec3 stdDevPos, physx::PxVec3 stdDevVel)
{
    pStdDevPos = stdDevPos;
    pStdDevVel = stdDevVel;

    vel_x = std::normal_distribution<double>{10, 4}; // generate normal numbers // axis x
    vel_y = std::normal_distribution<double>{0, 6}; // generate normal numbers // axis y
    vel_z = std::normal_distribution<double>{10, 4}; // generate normal numbers // axis z

	int max = 20;
	int min = 10;

	numPartPerExplosion = std::rand() % (max - min + 1) + min;
}

FireWorkParticleGenerator::~FireWorkParticleGenerator() {
	for (auto p : mParticles)
		delete p;

	mParticles.clear();
}

void FireWorkParticleGenerator::createFireWorkParticle(Particle* p) {
    mParticles.push_back(p);
}

void FireWorkParticleGenerator::integrate(double t) {
	std::list<Particle*>::iterator it = mParticles.begin();

	while (it != mParticles.end())
	{
		if (!(*it)->isAlive()) {
			// explotar
			explode((*it));

			delete* it;
			it = mParticles.erase(it);
		}
		else {
			(*it)->integrate(t);

			// (*it)->setColor(physx::PxVec4(color_r, color_g, color_b, 1));
			it++;
		}

	}
}

void FireWorkParticleGenerator::explode(Particle* parent) {
	if (parent->getGen() < 2) {
		for (int i = 0; i < numPartPerExplosion; i++) {
			Particle* n = new Particle(parent->getPos(), physx::PxVec3(vel_x(generator) - 20, vel_y(generator), vel_z(generator) - 20), physx::PxVec3(0, -3, 0), 1, 1, physx::PxVec4(1, 0, 0, 1), 2000, false);
			n->updateGen(parent->getGen() + 1);
			if (n->getGen() == 2) {
				n->setColor(physx::PxVec4(.3, .4, 1, 1));
			}
			else if (n->getGen() == 1) {
				n->setColor(physx::PxVec4(.7, .3, .4, 1));
			}
			mParticles.push_back(n);
		}
	}
}
