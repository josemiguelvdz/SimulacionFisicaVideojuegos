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
	sceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	mID = newID;

	switch (mID) {
	case 0:
		/*AddParticle(new Particle({ 0, 0, 0 }, { 0, 10, 0 }, { 0, 0, 0 }, 1, 1, { 0,0,1,1 }, 0, true));*/

		// 232, 127, 35
		PxRigidDynamic* sun = createRigidDynamic({ 0, 0, 0 }, gPhysics->createMaterial(0.0f, 0.0f, 0.0f), PxSphereGeometry(5), { (float)232/255, (float)127 / 255, (float)35 / 255, 1});
		sun->setAngularDamping(0);
		sun->setAngularVelocity({ 0, 2*3.14, 0 });
		//sun->addTorque({ 0.3, 1, -0.3 });

		sun->setMass(300);
		sun->setLinearVelocity({0,0,0});
		sun->setLinearDamping(0);

		PxRigidDynamic* earth = createRigidDynamic({ 40, 0, 40 }, gPhysics->createMaterial(0.0f, 0.0f, 0.0f), PxSphereGeometry(1.3f), { 0, 0, 1, 1 });
		earth->setAngularDamping(0);
		earth->setAngularVelocity({ 0, -2 * 3.14, 0 });
		earth->setMass(10);

		earth->setLinearDamping(0);

		PxRigidDynamic* mars = createRigidDynamic({ -70, 0, 40 }, gPhysics->createMaterial(0.0f, 0.0f, 0.0f), PxSphereGeometry(1), { 1, 0, 0, 1 });
		mars->setAngularDamping(0);
		mars->setAngularVelocity({ 0, -2 * 3.14, 0 });
		mars->setMass(5);

		mars->setLinearDamping(0);

		GravityField* gField = new GravityField(earth, sun, true);
		fg->AddRegistry(gField, earth);

		GravityField* gField2 = new GravityField(sun, earth, false);
		fg->AddRegistry(gField2, sun);

		GravityField* gField3 = new GravityField(mars, sun, true);
		fg->AddRegistry(gField3, mars);

		GravityField* gField4 = new GravityField(mars, earth, false);
		fg->AddRegistry(gField4, mars);

		GravityField* gField5 = new GravityField(earth, mars, false);
		fg->AddRegistry(gField5, earth);

		GravityField* gField6 = new GravityField(sun, mars, false);
		fg->AddRegistry(gField6, sun);

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

