#pragma once

#include "foundation/PxTransform.h"
#include "PxPhysicsAPI.h"
#include "RenderUtils.hpp"

#include <string>

class Particle
{
private:

	// Particle's transform properties
	physx::PxTransform mTransform;
	physx::PxVec3 mPos;

	// Particle's mass
	float mInverseMass;

	// Particle's velocity & acceleration
	physx::PxVec3 mVelocity;
	physx::PxVec3 mInitialVelocity;
	physx::PxVec3 mAcceleration;

	// Damping property
	double mDamping;

	// Particle's shape
	physx::PxShape* mShape;

	// Particle's color
	physx::PxVec4 mColor;

	// RenderItem pointer
	RenderItem* mRenderItem;

	// Particle's LifeTime
	float mMaxLifeTime;
	float mCurrLifeTime;
	bool mAlive;

	bool mStaticParticle;

public:
	// Constructor
	Particle(physx::PxVec3 pos, physx::PxVec3 vel, physx::PxVec3 acc, double damping, float scale, physx::PxVec4 color, float lifeTime, bool staticParticle);

	Particle(physx::PxVec3 pos, physx::PxVec3 vel, bool staticParticle);
	Particle(Particle* p);

	~Particle();

	// Integration
	void integrate(float t);

	// Return mAlive
	bool isAlive();

	// string name
	std::string mName;
	void setName(std::string newName);

	// Modify params
	Particle* setVel(physx::PxVec3 vel);
	physx::PxVec3 getVel();


	Particle* setPos(physx::PxVec3 pos);

	Particle* setAcc(physx::PxVec3 acc);
	Particle* setDamp(double damp);
	Particle* setIMass(double iMass);
	Particle* setColor(physx::PxVec4 color);
	Particle* setShape(physx::PxShape* shape);
	Particle* setLifeTime(float lifeTime);
	Particle* setAlpha(float alpha);
	Particle* setScale(float scale);

};

