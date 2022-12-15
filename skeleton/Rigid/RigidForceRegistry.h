#pragma once
#include "PxPhysicsAPI.h"
#include "foundation/PxTransform.h"
#include "RigidForceGenerator.h"

#include <math.h>
#include <climits>
#include <unordered_map>;

#include "RigidTorbellinoFGenerator.h"
#include "../ProyectoFinal/GravityField.h"

using namespace physx;

typedef std::unordered_multimap<RigidForceGenerator*, PxRigidDynamic*> mapFR;

class RigidForceRegistry : public mapFR
{
public:
	void Integrate(double dt);
	mapFR::iterator AddRegistry(RigidForceGenerator* fg, PxRigidDynamic* p);

	void DeleteParticle(PxRigidDynamic* p);
	void DeleteForce(RigidForceGenerator* fg);
};

