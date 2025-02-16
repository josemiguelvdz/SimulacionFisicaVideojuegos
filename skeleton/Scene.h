#pragma once

#include "PxPhysicsAPI.h"
#include "foundation/PxTransform.h"
#include "RenderUtils.hpp"
#include "core.hpp"
#include "callbacks.hpp"

#include "./Rigid/RigidParticleGenerator.h"
#include "./Rigid/RigidForceRegistry.h"
#include "./ParticleSystem.h"
#include "./SpaceParticleGenerator.h"
#include "./ProyectoFinal/PoolObjects.h"
#include "./UniformParticleGenerator.h"

#include <vector>

using namespace physx;

class Scene
{
public:
	Scene(PxDefaultCpuDispatcher* dispatcher, PxPhysics* physics, Camera* camera);
	~Scene();

	void LoadScene(int newID);
	void Update(double t);

	PxScene* getActiveScene() { return gScene; };
	PxPhysics* getActivePhysics() { return gPhysics; };

	PxRigidStatic* createRigidStatic(const physx::PxVec3& pos, PxMaterial* material, const PxGeometry& geo, const PxVec4& color);
	PxRigidDynamic* createRigidDynamic(const physx::PxVec3& pos, PxMaterial* material, const PxGeometry& geo, const PxVec4& color, double lifeTime, float scale);

	PoolObjects* getPoolObjects() { return pPoolObjects; };
	ParticleSystem* getPSystem() { return pSystem; };

	int getID() { return mID; };

	void generateWaterPlanet();
	void generateTornados(physx::PxVec3 tornadoPos, int sentido);
	void generateCube();
	void generateRandomRigid();
	void ZoomToEarth();

	/* Lerp Member Function
	Calculates the lerp for the vector and vector supplied using unit interval t */
	physx::PxVec3 lerp(const physx::PxVec3& s, const physx::PxVec3& e, float t) {
		return physx::PxVec3(s.x + (e.x - s.x) * t,
			s.y + (e.y - s.y) * t,
			s.z + (e.z - s.z) * t);
	}

private:
	// Scene
	int mID = 0;

	Camera* mCamera = nullptr;

	PxPhysics* gPhysics = nullptr;
	PxScene* gScene = nullptr;
	PxDefaultCpuDispatcher* gDispatcher = nullptr;

	ContactReportCallback gContactReportCallback;

	// PFinal
	ParticleSystem* pSystem = nullptr;
	PoolObjects* pPoolObjects = nullptr;

	// Escena agua
	GravityForceGenerator* gf = nullptr;
	WindForceGenerator* w = nullptr;
	BuoyancyForceGenerator* b = nullptr;

	// Escena agua rigidos
	RigidWindFGenerator* wRigids = nullptr;
	RigidBuoyancyFGenerator* bRigids = nullptr;
	RigidTorbellinoFGenerator* tRigids = nullptr;

	// Zoom
	bool mZoomActivated = false;
	bool mGoToCamera = false;
};

