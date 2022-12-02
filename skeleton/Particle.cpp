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

	mGenId = 0;

	clearForce();
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

	clearForce();
}

Particle::Particle(Particle* p)
{
	mPos = p->mPos;
	mVelocity = p->mVelocity;
	mAcceleration = p->mAcceleration;

	mDamping = p->mDamping;

	mInverseMass = p->mInverseMass;
	mMass = p->mMass;
	mForce = p->mForce;

	mColor = p->mColor;

	mTransform = physx::PxTransform(mPos);
	mShape = p->mShape;
	mRenderItem = new RenderItem(mShape, &mTransform, mColor);

	mMaxLifeTime = clock() + p->mMaxLifeTime;
	mAlive = true;
	mStaticParticle = p->mStaticParticle;

	clearForce();
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

	mAcceleration = mForce * mInverseMass;

	// Clear Force
	clearForce();

	// Update linear  velocity
	mVelocity += mAcceleration * t;

	// Update position
	mTransform = physx::PxTransform(mTransform.p.x + mVelocity.x * t, mTransform.p.y + mVelocity.y * t, mTransform.p.z + mVelocity.z * t);

	// Impose drag (damping)
	mVelocity *= powf(mDamping, t);

	// Life Time
	mCurrLifeTime = clock();

	if (!mStaticParticle && mCurrLifeTime > mMaxLifeTime)
		mAlive = false;
}

bool Particle::isAlive() {
	return mAlive;
}

Particle* Particle::setVel(physx::PxVec3 vel)
{
	mVelocity = vel;
	return this;
}

physx::PxVec3 Particle::getVel() {
	return mVelocity;
}

void Particle::setName(std::string newName) {
	mName = newName;
}

Particle* Particle::setAcc(physx::PxVec3 acc)
{
	mAcceleration = acc;
	return this;
}

Particle* Particle::setPos(physx::PxVec3 pos)
{
	mPos = pos;
	mTransform = physx::PxTransform(mPos);
	return this;
}

physx::PxVec3 Particle::getPos() {
	return mTransform.p;
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

Particle* Particle::setLifeTime(float lifeTime)
{
	mCurrLifeTime = clock();
	mMaxLifeTime = mCurrLifeTime + lifeTime;
	return this;
}

Particle* Particle::setAlpha(float alpha)
{
	
	return this;
}

void Particle::clearForce()
{
	mForce = physx::PxVec3(0);
}

void Particle::addForce(const physx::PxVec3& f)
{
	if(canRecForce)
		mForce += f;
}

Particle* Particle::setColor(physx::PxVec4 color)
{
	mColor = color;
	mRenderItem->color = color;
	return this;
}

//Particle* Particle::setScale(float scale)
//{
//	mScale
//	mRenderItem->color = color;
//	return this;
//}

