#include "GravityField.h"
#include <iostream>

GravityField::GravityField(PxRigidDynamic* _body1, PxRigidDynamic* _body2, bool hasInitialVelocity)
{
	body1 = _body1;
	body2 = _body2;

	// Velocidad Inicial
	if (hasInitialVelocity) {
		PxVec3 distance = body1->getGlobalPose().p - body2->getGlobalPose().p;
		PxVec3 res = pVectorial(distance, { 0, 1, 0 });
		body1->setLinearVelocity({ iniVel * res.x, iniVel * res.y , iniVel * res.z });
	}
}

GravityField::~GravityField()
{
}

void GravityField::updateForce(PxRigidDynamic* body1, double t)
{
	if (body1 != nullptr && body2 != nullptr && mActive) {
		PxVec3 distance = body1->getGlobalPose().p - body2->getGlobalPose().p;
		// std::cout << "Dist pre ---> " << distance.x << ":" << distance.y << ":" << distance.z << "\n";
		float modulo = distance.magnitude();

		distance.normalize();
		// std::cout << "Dist post ---> " << distance.x << ":" << distance.y << ":" << distance.z << "\n";

		float fuerzaG = (GCONSTANT * body1->getMass() * body2->getMass()) / pow(modulo, 2);

		PxVec3 vFuerza = distance * -fuerzaG;
		body1->addForce(vFuerza);

		//std::cout << "Vel ---> " << body1->getLinearVelocity().x << ":" << body1->getLinearVelocity().y << ":" << body1->getLinearVelocity().z << "\n";
		//
		//
		//std::cout << "Dist ---> " << distance.x << ":" << distance.y << ":" << distance.z << "\n";
		//
		//std::cout << "R ----> " << sqrt(pow(distance.x, 2) + pow(distance.y, 2) + pow(distance.z, 2)) << "\n";

		//std::cout << "F ---> " << vFuerza.x << ":" << vFuerza.y << ":" << vFuerza.z << "\n";

	}
}

PxVec3 GravityField::pVectorial(PxVec3 v1, PxVec3 v2)
{
	PxVec3 res;
	res.x = v1.y * v2.z - v1.z * v2.y;
	res.y = v1.z * v2.x - v1.x * v2.z;
	res.z = v1.x * v2.y - v1.y * v2.x;
	res.normalize();
	std::cout << "RES ----> " << res.x << ":" << res.y << ":" << res.z << "\n";
	return res;
}
