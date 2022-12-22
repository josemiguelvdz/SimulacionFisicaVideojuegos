#pragma once
#include "PxPhysicsAPI.h"
#include "foundation/PxTransform.h"
#include "RigidForceGenerator.h"

#include <math.h>
#include <list>
#include <climits>
#include <unordered_map>;

#include "../ProyectoFinal/RigidTorbellinoFGenerator.h"
#include "../ProyectoFinal/RigidWindFGenerator.h"
#include "../ProyectoFinal/RigidBuoyancyFGenerator.h"
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

	void addForceToData(RigidForceGenerator* f) { mAllForcesRigids.push_back(f); }
	std::vector<RigidForceGenerator*> getAllForces() { return mAllForcesRigids; };

private:
	std::vector<RigidForceGenerator*> mAllForcesRigids;
};

