#include "Particle.h"

Particle::Particle(physx::PxVec3 pos, physx::PxVec3 vel, physx::PxVec3 acc, double damping)
{
	mPos = pos;
	mVelocity = vel;
	mAcceleration = acc;

	mDamping = damping;

	mTransform = physx::PxTransform(pos.x, pos.y, pos.z);
	mRenderItem = new RenderItem(CreateShape(physx::PxSphereGeometry(1.0)), &mTransform, {0.5, 0.5, 1, 1});
}

Particle::~Particle()
{
	DeregisterRenderItem(mRenderItem);

	// delete mShape;
	delete mRenderItem;
}

void Particle::integrate(float t)
{
	mTransform = physx::PxTransform(mTransform.p.x + mVelocity.x * t, mTransform.p.y + mVelocity.y * t, mTransform.p.z + mVelocity.z * t);
	// Trivial  case, infinite mass --> do nothing
	// if (inverse_mass <=  0.0f) return;

	// Update position
	// mTransform += mVelocity * t;
	// Update linear  velocity
	// mVelocity += a * t;
	// 
	// Impose drag (damping)
	mVelocity *= powf(mDamping, t);
}
