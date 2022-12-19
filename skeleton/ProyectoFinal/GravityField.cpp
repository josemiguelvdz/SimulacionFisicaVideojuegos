#include "GravityField.h"
#include <iostream>

GravityField::GravityField(PxRigidDynamic* _body1, PxRigidDynamic* _body2, bool hasInitialVelocity, float iniVel)
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
		float modulo = distance.magnitude();
		distance.normalize();

		float fuerzaG = (GCONSTANT * body1->getMass() * body2->getMass()) / pow(modulo, 2);

		PxVec3 vFuerza = distance * -fuerzaG;
		body1->addForce(vFuerza);
	}
}

PxVec3 GravityField::pVectorial(PxVec3 v1, PxVec3 v2)
{
	PxVec3 res;
	res.x = v1.y * v2.z - v1.z * v2.y;
	res.y = v1.z * v2.x - v1.x * v2.z;
	res.z = v1.x * v2.y - v1.y * v2.x;
	res.normalize();
	return res;
}
