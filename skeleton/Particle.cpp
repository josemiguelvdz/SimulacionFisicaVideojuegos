#include "Particle.h"
#include <iomanip>

Particle::Particle(physx::PxVec3 pos, physx::PxVec3 vel, physx::PxVec3 acc, double damping, float scale, physx::PxVec4 color, float lifeTime, bool staticParticle)
{
	mPos = pos;
	mVelocity = vel;
	mAcceleration = acc;

	mDamping = damping;

	mInverseMass = 1;

	mTransform = physx::PxTransform(pos.x, pos.y, pos.z);
	mShape = CreateShape(physx::PxSphereGeometry(scale));
	mRenderItem = new RenderItem(mShape, &mTransform, color);

	mMaxLifeTime = clock() + lifeTime;

	mAlive = true;
	mStaticParticle = staticParticle;
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

	// Life Time
	mCurrLifeTime = clock();


	if (!mStaticParticle && mCurrLifeTime > mMaxLifeTime)
		mAlive = false;
}

bool Particle::isAlive() {
	return mAlive;
}
