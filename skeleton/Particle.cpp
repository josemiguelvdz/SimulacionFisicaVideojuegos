#include "Particle.h"

Particle::Particle(physx::PxVec3 pos, physx::PxVec3 vel, physx::PxVec3 acc, double damping, float scale)
{
	mPos = pos;
	mVelocity = vel;
	mAcceleration = acc;

	mDamping = damping;

	mInverseMass = 1;

	mTransform = physx::PxTransform(pos.x, pos.y, pos.z);
	mShape = CreateShape(physx::PxSphereGeometry(scale));
	mRenderItem = new RenderItem(mShape, &mTransform, {0.5, 0.5, 1, 1});
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
}
