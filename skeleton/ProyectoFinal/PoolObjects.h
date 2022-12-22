#pragma once
#include "../core.hpp"
#include <PxPhysicsAPI.h>
#include "../RenderUtils.hpp"
#include <unordered_map>

#include "../Rigid/RigidForceRegistry.h"
#include "../ProyectoFinal/RigidInfo.h"

using namespace physx;

// <Object, RenderItem>
typedef std::unordered_map<PxRigidDynamic*, RenderItem*> mapRenderItems;

class PoolObjects
{
public:
	PoolObjects();

	void Update(double t);

	void clearScene();

	void addRenderItem(PxRigidDynamic* p, RenderItem* i);
	void deleteRenderItem(PxRigidDynamic* p);
	RenderItem* getRenderItem(PxRigidDynamic* p);

	RigidForceRegistry* getForceRegistry() { return mForceRegistry; };

	PxRigidDynamic* getSun();
	PxRigidDynamic* getEarth();


	void setActiveScene(PxScene* scene) { mActiveScene = scene; }

	void checkIfDead();
	void addLifeTime(double t);
	void explode(PxVec3 iniPos);

	void clearForcesInAllRigids() { 
		for (auto it : mRenderItemsMap) {
			it.first->setLinearVelocity({0, 0, 0});
			it.first->clearForce();
			// it.first.set
		}
			
		mForceRegistry->clear();
	};
	
private:
	mapRenderItems mRenderItemsMap;
	RigidForceRegistry* mForceRegistry = nullptr;
	
	PxScene* mActiveScene = nullptr;
};

