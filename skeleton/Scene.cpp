#include "Scene.h"

Scene::Scene(PxDefaultCpuDispatcher* dispatcher, PxPhysics* physics)
{
	gDispatcher = dispatcher;
	gPhysics = physics;

	fg = new RigidForceRegistry();

	LoadScene(0);
}

Scene::~Scene()
{
	if (gScene != nullptr)
		gScene->release();

	if (gPhysics != nullptr)
		gPhysics->release();
}

void Scene::LoadScene(int newID)
{
	if (gScene != nullptr)
		gScene->release();

	for (auto r : gRenderItems)
		r->release();

	gRenderItems.clear();
	fg->clear();
	gForceGenerators.clear();

	delete rGen;
	rGen = nullptr;


	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	mID = newID;

	RigidTorbellinoFGenerator* t = nullptr;

	switch (mID) {
	case 0:
		/*AddParticle(new Particle({ 0, 0, 0 }, { 0, 10, 0 }, { 0, 0, 0 }, 1, 1, { 0,0,1,1 }, 0, true));*/
		createRigidStatic({ 30, 30, 30 }, gPhysics->createMaterial(0.5f, 0.5f, 0.6f), PxBoxGeometry(60, 0.5, 60), { 0, 1, 0, 1 });

		createRigidStatic({ 30, 25, 30 }, gPhysics->createMaterial(0.5f, 0.5f, 0.6f), PxSphereGeometry(15), { 1, 1, 0, 1 });

		createRigidStatic({ 10, 50, 30 }, gPhysics->createMaterial(0.5f, 0.5f, 0.6f), PxSphereGeometry(10), { 1, 1, 0, 1 });

		rGen = new RigidParticleGenerator({ 30, 30, 30 }, { 0, 10, 0 }, this);
		break;
	case 1:
		// createRigidDynamic({ 0, 0, 0 }, gPhysics->createMaterial(0.5f, 0.5f, 0.6f), PxSphereGeometry(1));
		t = new RigidTorbellinoFGenerator({ 30, 30, 30 }, 120, 5);
		gForceGenerators.push_back(t);

		createRigidStatic({ 30, 30, 30 }, gPhysics->createMaterial(0.5f, 0.5f, 0.6f), PxBoxGeometry(60, 0.5, 60), { 0, 1, 0, 1 });
		createRigidStatic({ 30, 25, 30 }, gPhysics->createMaterial(0.5f, 0.5f, 0.6f), PxSphereGeometry(15), { 1, 1, 0, 1 });

		rGen = new RigidParticleGenerator({ 30, 30, 30 }, { 0, 20, 0 }, this);
		break;
	case 2:
		break;
	}
}

void Scene::Update(double t)
{
	// Force Registry u otras cosas
	if (rGen != nullptr && rGen->getParticleCounter() < 100) // Siendo 100 el limite de particulas
		rGen->generateParticles();

	fg->Integrate(t);
}



//void Scene::ClearScene()
//{
//	fg.clear();	
//}


physx::PxRigidStatic* Scene::createRigidStatic(const physx::PxVec3& pos, PxMaterial* material, const PxGeometry& geo, const PxVec4& color)
{
	physx::PxRigidStatic* particle = gPhysics->createRigidStatic(physx::PxTransform(pos));
	physx::PxShape* shape = gPhysics->createShape(geo, *material);
	// shape->setMaterials(&gMaterial, physx::PxU16(1));
	particle->attachShape(*shape);

	gRenderItems.push_back(new RenderItem(shape, particle, color));

	gScene->addActor(*particle);
	return particle;
}

physx::PxRigidDynamic* Scene::createRigidDynamic(const physx::PxVec3& pos, PxMaterial* material, const PxGeometry& geo, const PxVec4& color)
{
	// Create Rigid Dynamic
	physx::PxRigidDynamic* particle = gPhysics->createRigidDynamic(physx::PxTransform(pos));

	// Material & shape (DEFAULT)
	/*PxMaterial* gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);*/
	physx::PxShape* shape = gPhysics->createShape(geo, *material);
	particle->attachShape(*shape);

	// Register Render Item
	gRenderItems.push_back(new RenderItem(shape, particle, color));

	// Test Properties
	// particle->setLinearVelocity({ 0, 10, 0 });
	// particle->setAngularVelocity({ 0, 0, 0 });

	// Add Actor to the scene
	gScene->addActor(*particle);

	for (auto forceGenerator : gForceGenerators)
		fg->AddRegistry(forceGenerator, particle);

	return particle;
}

