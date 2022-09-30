#include "Particle.h"
#include <iostream>
#include <iomanip>

Particle::Particle(physx::PxVec3 pos, physx::PxVec3 vel, physx::PxVec3 acc, double damping, float scale, physx::PxVec4 color, float lifeTime, bool staticParticle)
{
	mPos = pos;
	mVelocity = vel;
	mAcceleration = acc;

	mDamping = damping;

	mInverseMass = 1;

	mColor = color;

	mTransform = physx::PxTransform(pos.x, pos.y, pos.z);
	mShape = CreateShape(physx::PxSphereGeometry(scale));
	mRenderItem = new RenderItem(mShape, &mTransform, mColor);

	mMaxLifeTime = clock() + lifeTime;

	mAlive = true;
	mStaticParticle = staticParticle;
}

Particle::Particle(physx::PxVec3 pos, physx::PxVec3 vel, bool staticParticle)
{
	mPos = pos;
	mVelocity = vel;

	mAlive = true;

	mStaticParticle = staticParticle;
	mMaxLifeTime = clock() + 10000;

	// Default
	mTransform = physx::PxTransform(pos.x, pos.y, pos.z);
	mShape = CreateShape(physx::PxSphereGeometry(1.0));
	mRenderItem = new RenderItem(mShape, &mTransform, mColor);
}

Particle::~Particle()
{
	DeregisterRenderItem(mRenderItem);
	// mShape->release();

	// delete mShape;
	// delete mRenderItem;
}

void Particle::integrate(float t)
{
	// Trivial  case, infinite mass --> do nothing
	if (mInverseMass <= 0.0f)
		return;

	// Update position
	mTransform = physx::PxTransform(mTransform.p.x + mVelocity.x * t, mTransform.p.y + mVelocity.y * t, mTransform.p.z + mVelocity.z * t);

	// Update linear  velocity
	mVelocity += mAcceleration * t;

	// Impose drag (damping)
	mVelocity *= powf(mDamping, t);

	
	// test
	//mTransform.p += mVelocity * t;
	//mVelocity += mAcceleration * t;
	//mVelocity *= pow(mDamping, t);

	// Life Time
	mCurrLifeTime = clock();

	if (!mStaticParticle && mCurrLifeTime > mMaxLifeTime)
		mAlive = false;

	std::cout << "Particle's Vel x: " << mVelocity.y << "\n";
}

bool Particle::isAlive() {
	return mAlive;
}

Particle* Particle::setVel(physx::PxVec3 vel)
{
	mVelocity = vel;
	return this;
}
Particle* Particle::setAcc(physx::PxVec3 acc)
{
	mAcceleration = acc;
	return this;
}

Particle* Particle::setDamp(double damp)
{
	mDamping = damp;
	return this;
}

Particle* Particle::setIMass(double iMass)
{
	mInverseMass = iMass;
	return this;
}

Particle* Particle::setShape(physx::PxShape* shape)
{
	mShape = shape;
	mRenderItem->shape = shape;
	return this;
}

Particle* Particle::setColor(physx::PxVec4 color)
{
	mColor = color;
	mRenderItem->color = color;
	return this;
}

