#include "PoolObjects.h"
#include <iostream>

PoolObjects::PoolObjects()
{
	mForceRegistry = new RigidForceRegistry();
}

void PoolObjects::Update(double t)
{
	// Sumar Life Time
	addLifeTime(t);

	// Mirar si murieron
	checkIfDead();

	// Actualizar los rigids
	mForceRegistry->Integrate(t);

	mActiveScene->simulate(t);
	mActiveScene->fetchResults(true);
}



void PoolObjects::clearScene()
{
	for (auto it = mRenderItemsMap.begin(); it != mRenderItemsMap.end();) { // Borro el renderItem
		it->second->release();
		it++;
	}

	mRenderItemsMap.clear();

	for (auto it = mForceRegistry->begin(); it != mForceRegistry->end();) { // Borro el renderItem
		delete it->first;
		it++;
	}
		
	mForceRegistry->clear();

	if(mActiveScene != nullptr)
		mActiveScene->release();
}

void PoolObjects::addRenderItem(PxRigidDynamic* p, RenderItem* i)
{
	mRenderItemsMap.insert({ p, i });
}

void PoolObjects::deleteRenderItem(PxRigidDynamic* p)
{
	for (auto it = mRenderItemsMap.begin(); it != mRenderItemsMap.end();)
		if (it->first == p) {
			if (it->second != nullptr)
				it->second->release();
			it = mRenderItemsMap.erase(it);

			break;
		}	
		else
			it++;
}

void PoolObjects::addLifeTime(double t) {
	for (auto it = mRenderItemsMap.begin(); it != mRenderItemsMap.end();) {
		if (it->first != nullptr)
			((RigidInfo*)it->first->userData)->actLifeTime += t;
		it++;
	}
}

void PoolObjects::checkIfDead()
{
	for (auto it = mRenderItemsMap.begin(); it != mRenderItemsMap.end();)
		if (it->first != nullptr) {
			if (((RigidInfo*)it->first->userData)->pStatus == S_DEAD || ((RigidInfo*)it->first->userData)->actLifeTime > ((RigidInfo*)it->first->userData)->maxLifeTime) {
				it->second->release();
				it = mRenderItemsMap.erase(it);
			}
			else
				it++;
		}
}
