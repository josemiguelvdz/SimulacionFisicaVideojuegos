#pragma once

#include "foundation/PxTransform.h"
#include "PxPhysicsAPI.h"
#include "RenderUtils.hpp"

class Plane
{
private:

	// Particle's transform properties
	physx::PxTransform mTransform;
	physx::PxVec3 mPos;

	// Particle's shape
	physx::PxShape* mShape;

	// RenderItem pointer
	RenderItem* mRenderItem;

public:
	// Constructor
	Plane(physx::PxVec3 pos);
	~Plane();

	// Integration
	// void integrate(double t);
};

