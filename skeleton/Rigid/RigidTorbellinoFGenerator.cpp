#include "RigidTorbellinoFGenerator.h"
#include <iostream>

RigidTorbellinoFGenerator::RigidTorbellinoFGenerator(const physx::PxVec3& center, double rad, double force)
{
	mCenter = center;
	mRad = rad;
	mForce = force;
}

void RigidTorbellinoFGenerator::updateForce(PxRigidDynamic* particle, double t)
{
	if (particle != nullptr && mActive) {
		if (fabs(particle->getInvMass()) < 1e-10 || !isInsideRange(particle))
			return;

		particle->addForce(mWindFriction * mForce * torbellinoVel(particle->getGlobalPose().p) +
			mWindFriction2 * torbellinoPow(mForce * torbellinoVel(particle->getGlobalPose().p), 2));
	}
}

PxVec3 RigidTorbellinoFGenerator::torbellinoVel(PxVec3 pos) {

	PxVec3 vel = pos - mCenter;
	vel = vel.cross(PxVec3(0, 1, 0)) + 5 * -vel;
	vel.y = 0;
	return vel;
}

bool RigidTorbellinoFGenerator::isInsideRange(PxRigidDynamic* p) {
	
	PxVec3 vel(p->getLinearVelocity());
	//std::cout << "Vel --->" << sqrt(pow(vel.x, 2) + pow(vel.z, 2)) << "\n";
	//std::cout << "Pos --->" << sqrt(pow(p->getGlobalPose().p.x, 2) + pow(p->getGlobalPose().p.z, 2)) << "\n";
	
	return pow(p->getGlobalPose().p.x, 2) + pow(p->getGlobalPose().p.z, 2) <= pow(mRad, 2);
}
