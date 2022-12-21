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
	float mMass;
	float mInverseMass;

	// Particle's velocity & acceleration
	physx::PxVec3 mVelocity;
	physx::PxVec3 mInitialVelocity;
	physx::PxVec3 mAcceleration;

	// Forces
	physx::PxVec3 mForce;

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

	// Generation Id if needed
	int mGenId;

	// Wind Friction
	double mWindFriction = 1;
	double mWindFriction2 = 0;

	// Forces
	bool canRecForce = true;

	// FW
	bool bFirework = false;

	// T
	bool bTornado = false;

	// Scale
	float mScale = 0;
	float mIniScale = mScale;

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

	void addGen() { mGenId++; };
	int getGen() { return mGenId; };
	void updateGen(int newGen) { mGenId = newGen; };

	Particle* setPos(physx::PxVec3 pos);
	physx::PxVec3 getPos();

	Particle* setAcc(physx::PxVec3 acc);
	Particle* setDamp(double damp);

	Particle* setIMass(double iMass);
	float getIMass() { return mInverseMass; };

	Particle* setMass(float mass) { mMass = mass; return this; };
	float getMass() { return mMass; };

	Particle* setColor(physx::PxVec4 color);
	Particle* setShape(physx::PxShape* shape);
	Particle* setLifeTime(float lifeTime);
	Particle* setAlpha(float alpha);

	Particle* setScale(float scale);
	float getScale() { return mScale; };

	float getIniScale() { return mIniScale; };


	Particle* setWindFriction(double friction) { mWindFriction = friction; };
	Particle* setWindFriction2(double friction2) { mWindFriction2 = friction2; };

	double getWindFriction() { return mWindFriction; };
	double getWindFriction2() { return mWindFriction2; };

	void clearForce();
	void addForce(const physx::PxVec3& f);

	void setCanRecForce(bool recForce) { canRecForce = recForce; };

	void setFireWork(bool fw) { bFirework = fw; };
	bool isFirework() { return bFirework; };

	void setAffectedByTornado(bool t) { bTornado = t; };
	bool isAffectedByTornado() { return bTornado; };

	void setStaticParticle(bool state) { mStaticParticle = state; }

};

