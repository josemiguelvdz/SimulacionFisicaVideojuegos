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

#include <vector>

using namespace physx;


/* Robacion de codigo ajajjaj

bool pointSphere(Vector3 pointPos, const physx::PxTransform* sphereTransform) {
		return EXP_SPHERE_RADIUS > sqrt(pow(pointPos.x - sphereTransform->p.x, 2) + pow(pointPos.y - sphereTransform->p.y, 2) + pow(pointPos.z - sphereTransform->p.z, 2));
	}

public:

	ParticleExplosion(float force, RenderItem* eS) : expSphere(eS), f(force){}

	virtual void updateForce(Particula* particle, float t) {
		if (!explota) return;
		if (!particle->hasFiniteMass()) return;

		if (pointSphere(particle->getPosition(), expSphere->transform)) {
			Vector3 dir = particle->getPosition() - sphereExpPosition;
			float dist = dir.normalize();

			float force = (EXP_SPHERE_RADIUS - dist) / EXP_SPHERE_RADIUS * f;

			particle->addForce(dir * force / t);
		}

		explota = false;
		
		
		*/

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
	PxRigidDynamic* createRigidDynamic(const physx::PxVec3& pos, PxMaterial* material, const PxGeometry& geo, const PxVec4& color, double lifeTime);

	PoolObjects* getPoolObjects() { return pPoolObjects; };

private:
	// Scene
	int mID = 0;

	PxPhysics* gPhysics = nullptr;
	PxScene* gScene = nullptr;
	PxDefaultCpuDispatcher* gDispatcher = nullptr;

	ContactReportCallback gContactReportCallback;

	// PFinal
	ParticleSystem* pSystem = nullptr;
	PoolObjects* pPoolObjects = nullptr;

	int rigidCont = 0;
};

