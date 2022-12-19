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

	RigidForceRegistry* getForceRegistry() { return mForceRegistry; };
	void setActiveScene(PxScene* scene) { mActiveScene = scene; }

	void checkIfDead();
	void addLifeTime(double t);
	
private:
	mapRenderItems mRenderItemsMap;

	RigidForceRegistry* mForceRegistry = nullptr;

	PxScene* mActiveScene = nullptr;

};

