#include "Projectile.h"

Projectile::Projectile(physx::PxVec3 pos, physx::PxVec3 vel, physx::PxVec3 acc, double damping)
{
	mPos = pos;
	mVelocity = vel;
	mAcceleration = acc;

	mDamping = damping;

	mInverseMass = 1;

	mTransform = physx::PxTransform(pos.x, pos.y, pos.z);
	mShape = CreateShape(physx::PxSphereGeometry(1.0));
	mRenderItem = new RenderItem(mShape, &mTransform, { 0.5, 0.5, 1, 1 });
}

Projectile::~Projectile()
{
	DeregisterRenderItem(mRenderItem);
	// mShape->release();

	// delete mShape;
	// delete mRenderItem;
}

void Projectile::integrate(float t)
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
