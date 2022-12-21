#include "TorbellinoForceGenerator.h"
#include <iostream>

TorbellinoForceGenerator::TorbellinoForceGenerator(const physx::PxVec3& center, double rad, double force)
{
	mCenter = center;
	mRad = rad;
	mForce = force;
}

void TorbellinoForceGenerator::updateForce(Particle* particle, double t)
{
	if (particle != nullptr && mActive) {
		if (fabs(particle->getIMass()) < 1e-10 || !isInsideRange(particle))
			return;

		physx::PxVec3 torVel = torbellinoVel(particle->getPos(), particle->getVel(), t);

		if (particle->isAffectedByTornado())
			torVel.y = 10;

		physx::PxVec3 force = particle->getWindFriction() * mForce * torVel +
			particle->getWindFriction2() * torbellinoPow(mForce * torVel, 2);

		particle->addForce(force);
	}
}

physx::PxVec3 TorbellinoForceGenerator::torbellinoVel(physx::PxVec3 pos, physx::PxVec3 pVel, double t) {

	physx::PxVec3 vel = pos - mCenter;
	vel = vel.cross(physx::PxVec3(0, 1, 0)) + 5 * -vel;
	vel.y = 0;

	// std::cout << "vel: " << vel.x << " " << vel.y << " " << vel.z << std::endl;

	return vel;
}


bool TorbellinoForceGenerator::isInsideRange(Particle* p) {

	// physx::PxVec3 vel(p->getVel());
	//std::cout << "Vel --->" << sqrt(pow(vel.x, 2) + pow(vel.z, 2)) << "\n";
	//std::cout << "Pos --->" << sqrt(pow(p->getGlobalPose().p.x, 2) + pow(p->getGlobalPose().p.z, 2)) << "\n";
	// return true;
	// return pow(p->getPos().x, 2) + pow(p->getPos().z, 2) <= pow(mRad, 2);
	return pow(p->getPos().x - mCenter.x, 2) + pow(p->getPos().z - mCenter.z, 2) <= pow(mRad, 2);
}