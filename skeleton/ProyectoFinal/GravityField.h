#pragma once
#include "PxPhysicsAPI.h"
#include "foundation/PxTransform.h"

#include "../Rigid/RigidForceGenerator.h"

using namespace physx;

class GravityField : public RigidForceGenerator
{
public:
	GravityField(PxRigidDynamic* _body1, PxRigidDynamic* _body2, bool hasInitialVelocity);
	~GravityField();

	virtual void updateForce(PxRigidDynamic* body1, double t) override;

	PxVec3 pVectorial(PxVec3 v1, PxVec3 v2);
private:
	PxRigidDynamic* body1 = nullptr;
	PxRigidDynamic* body2 = nullptr;

	const double GCONSTANT = 10;
	float iniVel = 5;
};

