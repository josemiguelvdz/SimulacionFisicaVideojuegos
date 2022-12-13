#pragma once

#include "PxPhysicsAPI.h"
#include "foundation/PxTransform.h"
#include "RenderUtils.hpp"
#include "core.hpp"
#include "callbacks.hpp"

#include "./Rigid/RigidParticleGenerator.h"

#include <vector>

using namespace physx;

class Scene
{
public:
	Scene(PxDefaultCpuDispatcher* dispatcher, PxPhysics* physics);
	~Scene();

	void LoadScene(int newID);
	void Update(double t);

	PxScene* getActiveScene() { return gScene; };
	PxPhysics* getActivePhysics() { return gPhysics; };

	PxRigidStatic* createRigidStatic(const physx::PxVec3& pos, PxMaterial* material, const PxGeometry& geo, const PxVec4& color);
	PxRigidDynamic* createRigidDynamic(const physx::PxVec3& pos, PxMaterial* material, const PxGeometry& geo, const PxVec4& color);


private:
	// Scene
	int mID = 0;

	std::vector<RenderItem*> gRenderItems;

	PxPhysics* gPhysics = nullptr;
	PxScene* gScene = nullptr;
	PxDefaultCpuDispatcher* gDispatcher = nullptr;

	ContactReportCallback gContactReportCallback;

	// Practica 1
	RigidParticleGenerator* rGen = nullptr;
};

