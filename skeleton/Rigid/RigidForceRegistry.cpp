#include "RigidForceRegistry.h"
#include "RigidForceGenerator.h"

void RigidForceRegistry::Integrate(double dt)
{
	for (auto it = begin(); it != end(); it++) {
		if(it->second != nullptr)
			it->first->updateForce(it->second, dt);
	}
}

mapFR::iterator RigidForceRegistry::AddRegistry(RigidForceGenerator* fg, PxRigidDynamic* p)
{
	return insert({ fg, p });
}

void RigidForceRegistry::DeleteParticle(PxRigidDynamic* p)
{
	for (auto it = begin(); it != end();)
		if (it->second == p)
			it = erase(it);
		else
			it++;
}

void RigidForceRegistry::DeleteForce(RigidForceGenerator* fg)
{
	for (auto it = begin(); it != end();)
		if (it->first == fg)
			it = erase(it);
		else
			it++;
}

