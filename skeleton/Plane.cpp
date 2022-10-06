#include "Plane.h"
#include <iostream>

Plane::Plane(physx::PxVec3 pos)
{
	mTransform = physx::PxTransform(pos.x, pos.y, pos.z);

	mShape = CreateShape(physx::PxBoxGeometry(10, 0.5, 10));

	mRenderItem = new RenderItem(mShape, &mTransform, { 1, 0, 0, 1 });
}

Plane::~Plane()
{
	DeregisterRenderItem(mRenderItem);
}
