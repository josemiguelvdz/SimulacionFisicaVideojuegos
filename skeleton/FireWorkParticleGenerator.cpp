#include "FireWorkParticleGenerator.h"


FireWorkParticleGenerator::FireWorkParticleGenerator()
{
	vel_x = std::normal_distribution<double>{ 0.1, 0.05 }; // generate normal numbers // axis x
	vel_y = std::normal_distribution<double>{ 0.1, 0.05 }; // generate normal numbers // axis y
	vel_z = std::normal_distribution<double>{ 0.1, 0.05 }; // generate normal numbers // axis z

	int max = 30;
	int min = 20;

	numPartPerExplosion = std::rand() % (max - min + 1) + min;

	mParticles.clear();
}

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
	mParticles.clear();
}

FireWorkParticleGenerator::~FireWorkParticleGenerator() {
	//for (auto p : mParticles)
	//	delete p;

	//mParticles.clear();
}

//void FireWorkParticleGenerator::createFireWorkParticle(Particle* p) {
//    mParticles.push_back(p);
//}
//
//void FireWorkParticleGenerator::integrate(double t) {
//	std::list<Particle*>::iterator it = mParticles.begin();
//
//	while (it != mParticles.end())
//	{
//		if (*it != nullptr) {
//			// reducir tamaño
//			(*it)->setScale((*it)->getScale() - 0.15);
//			it++;
//		}
//		else { // borrar del array
//			it = mParticles.erase(it);			
//		}
//
//	}
//}
//
//void FireWorkParticleGenerator::explode(Particle* parent) {
//	if (parent->getGen() < 2) {
//		for (int i = 0; i < numPartPerExplosion; i++) {
//			Particle* n = new Particle(parent->getPos(), physx::PxVec3(vel_x(generator) - 20, vel_y(generator), vel_z(generator) - 20), physx::PxVec3(0, -3, 0), 1, 1, physx::PxVec4(1, 0, 0, 1), 2000, false);
//			n->updateGen(parent->getGen() + 1);
//			if (n->getGen() == 2) {
//				n->setColor(physx::PxVec4(.3, .4, 1, 1));
//			}
//			else if (n->getGen() == 1) {
//				n->setColor(physx::PxVec4(.7, .3, .4, 1));
//			}
//			mParticles.push_back(n);
//		}
//	}
//}

std::list<Particle*> FireWorkParticleGenerator::planetExplosion(physx::PxVec3& iniPos, physx::PxVec3& iniVel, float& iniScale) {
	std::list<Particle*> expParticles;

	for (int i = 0; i < numPartPerExplosion; i++) {
		Particle* n = new Particle(iniPos, physx::PxVec3(vel_x(generator) * -iniVel.x * iniScale * 0.3, vel_y(generator) * -iniVel.y * iniScale * 0.2, vel_z(generator)) * -iniVel.z * iniScale * 0.3,
			physx::PxVec3(0, 0, 0), 1, iniScale, physx::PxVec4(1, 0, 0, 1), 2.f, false);
		// n->updateGen(parent->getGen() + 1);
		n->setFireWork(true);

		n->setColor(physx::PxVec4(1, 1, 0.7, 1));
		expParticles.push_back(n);
	}

	return expParticles;
}
