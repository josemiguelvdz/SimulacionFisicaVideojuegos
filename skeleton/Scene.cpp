#include "Scene.h"

Scene::Scene(PxDefaultCpuDispatcher* dispatcher, PxPhysics* physics)
{
	gDispatcher = dispatcher;
	gPhysics = physics;

	pPoolObjects = new PoolObjects();

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
	pPoolObjects->clearScene();

	//if (gScene != nullptr)
	//	gScene->release();

	//for (auto r : gRenderItems)
	//	r->release();

	//gRenderItems.clear();
	//fg->clear();
	//gForceGenerators.clear();

	if (pSystem != nullptr) {
		for (auto it : pSystem->getParticles())
			delete it;

		pSystem->getParticles().clear();	
	}
		

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);

	pPoolObjects->setActiveScene(gScene);

	mID = newID;

	switch (mID) {
	case 0:
	{
		/*AddParticle(new Particle({ 0, 0, 0 }, { 0, 10, 0 }, { 0, 0, 0 }, 1, 1, { 0,0,1,1 }, 0, true));*/

		// 232, 127, 35
		PxRigidDynamic* sun = createRigidDynamic({ 0, 0, 0 }, gPhysics->createMaterial(0.0f, 0.0f, 0.0f), PxSphereGeometry(5), { (float)232/255, (float)127 / 255, (float)35 / 255, 1}, INT_MAX);
		sun->setAngularDamping(0);
		sun->setAngularVelocity({ 0, 1, 0 });
		//sun->addTorque({ 0.3, 1, -0.3 });

		sun->setMass(200);
		sun->setLinearVelocity({0,0,0});
		sun->setLinearDamping(0);

		PxRigidDynamic* earth = createRigidDynamic({ 55, 0, 40 }, gPhysics->createMaterial(0.0f, 0.0f, 0.0f), PxSphereGeometry(1.7f), { 0, 0, 1, 1 }, INT_MAX);
		earth->setAngularDamping(0);
		earth->setAngularVelocity({ 0, -2 * 3.14, 0 });
		earth->setMass(10);

		earth->setLinearDamping(0);

		PxRigidDynamic* mars = createRigidDynamic({ 50, 0, -40 }, gPhysics->createMaterial(0.0f, 0.0f, 0.0f), PxSphereGeometry(0.8), { 1, 0, 0, 1 }, INT_MAX);
		mars->setAngularDamping(0);
		mars->setAngularVelocity({ 0, -2 * 3.14, 0 });
		mars->setMass(0.2);

		mars->setLinearDamping(0);

		PxRigidDynamic* pluto = createRigidDynamic({ 5, 0, 10 }, gPhysics->createMaterial(0.0f, 0.0f, 0.0f), PxSphereGeometry(0.6), { 0, 1, 0, 1 }, INT_MAX);
		pluto->setAngularDamping(0);
		pluto->setAngularVelocity({ 0, -2 * 3.14, 0 });
		pluto->setMass(0.1);

		pluto->setLinearDamping(0);


		/// EARTH

		GravityField* gField = new GravityField(earth, sun, true, 5); // Tierra y Sol
		gField->setGConstant(10);
		pPoolObjects->getForceRegistry()->AddRegistry(gField, earth);

		GravityField* gField5 = new GravityField(earth, mars, false, 0); // Tierra y Marte
		gField5->setGConstant(30);
		pPoolObjects->getForceRegistry()->AddRegistry(gField5, earth);

		GravityField* gField12 = new GravityField(earth, pluto, false, 0); // Tierra y Pluto
		gField12->setGConstant(30);
		pPoolObjects->getForceRegistry()->AddRegistry(gField12, earth);


		/// MARS

		GravityField* gField3 = new GravityField(mars, sun, true, 6); // Marte y Sol
		gField3->setGConstant(40);
		pPoolObjects->getForceRegistry()->AddRegistry(gField3, mars);

		GravityField* gField10 = new GravityField(mars, pluto, false, 0); // Marte y Pluto
		gField10->setGConstant(30);
		pPoolObjects->getForceRegistry()->AddRegistry(gField10, mars);

		GravityField* gField4 = new GravityField(mars, earth, false, 0); // Marte y Tierra
		gField4->setGConstant(30);
		pPoolObjects->getForceRegistry()->AddRegistry(gField4, mars);



		/// PLUTO

		GravityField* gField7 = new GravityField(pluto, sun, true, 20); // Pluto y sol
		gField7->setGConstant(30);
		pPoolObjects->getForceRegistry()->AddRegistry(gField7, pluto);

		GravityField* gField9 = new GravityField(pluto, mars, false, 0); // Pluto y Marte
		gField9->setGConstant(30);
		pPoolObjects->getForceRegistry()->AddRegistry(gField9, pluto);

		GravityField* gField13 = new GravityField(pluto, earth, false, 0); // Pluto y Tierra
		gField13->setGConstant(30);
		pPoolObjects->getForceRegistry()->AddRegistry(gField13, pluto);


		/// SUN

		GravityField* gField2 = new GravityField(sun, earth, false, 0); // Sol y Tierra
		pPoolObjects->getForceRegistry()->AddRegistry(gField2, sun);

		GravityField* gField6 = new GravityField(sun, mars, false, 0); // Sol y Marte
		pPoolObjects->getForceRegistry()->AddRegistry(gField6, sun);

		GravityField* gField8 = new GravityField(sun, pluto, false, 0); // Sol y Pluto
		pPoolObjects->getForceRegistry()->AddRegistry(gField8, sun);


		/// STARS
		pSystem = new ParticleSystem();

		SpaceParticleGenerator* stars = new SpaceParticleGenerator(physx::PxVec3(200, 0, 200), physx::PxVec3(0, 0, 0));
		stars->setMeanParticles(2);
		stars->setParticle(new Particle({ 2000, 0, 2000 }, { 0, 0, 0 }, { 0, -.1, 0 }, 1, 0.5, { 1, 1, 0.7, 1 }, 15000, true));

		pSystem->addParticleGenerator(stars);

	}
		break;

	case 1:
		/*AddParticle(new Particle({ 0, 0, 0 }, { 0, 10, 0 }, { 0, 0, 0 }, 1, 1, { 0,0,1,1 }, 0, true));*/
	{
		PxRigidDynamic * sun = createRigidDynamic({ 0, 0, 0 }, gPhysics->createMaterial(0.0f, 0.0f, 0.0f), PxSphereGeometry(5), { (float)232 / 255, (float)127 / 255, (float)35 / 255, 1 }, INT_MAX);
		sun->setAngularDamping(0);
		sun->setAngularVelocity({ 0, 1, 0 });
		//sun->addTorque({ 0.3, 1, -0.3 });

		sun->setMass(220);
		sun->setLinearVelocity({ 0,0,0 });
		sun->setLinearDamping(0);

		PxRigidDynamic* earth = createRigidDynamic({ 30, 0, 40 }, gPhysics->createMaterial(0.0f, 0.0f, 0.0f), PxSphereGeometry(1.3f), { 0, 0, 1, 1 }, INT_MAX);
		earth->setAngularDamping(0);
		earth->setAngularVelocity({ 0, -2 * 3.14, 0 });
		earth->setMass(10);

		earth->setLinearDamping(0);

		PxRigidDynamic* mars = createRigidDynamic({ -10, 0, -15 }, gPhysics->createMaterial(0.0f, 0.0f, 0.0f), PxSphereGeometry(0.8), { 1, 0, 0, 1 }, INT_MAX);
		mars->setAngularDamping(0);
		mars->setAngularVelocity({ 0, -2 * 3.14, 0 });
		mars->setMass(0.2);

		mars->setLinearDamping(0);

		PxRigidDynamic* pluto = createRigidDynamic({ 10, 0, 15 }, gPhysics->createMaterial(0.0f, 0.0f, 0.0f), PxSphereGeometry(0.6), { 0, 1, 0, 1 }, INT_MAX);
		pluto->setAngularDamping(0);
		pluto->setAngularVelocity({ 0, -2 * 3.14, 0 });
		pluto->setMass(0.1);

		pluto->setLinearDamping(0);


		/// EARTH

		GravityField* gField = new GravityField(earth, sun, true, 7); // Tierra y Sol
		gField->setGConstant(30);
		pPoolObjects->getForceRegistry()->AddRegistry(gField, earth);

		GravityField* gField5 = new GravityField(earth, mars, false, 0); // Tierra y Marte
		gField5->setGConstant(30);
		pPoolObjects->getForceRegistry()->AddRegistry(gField5, earth);

		GravityField* gField12 = new GravityField(earth, pluto, false, 0); // Tierra y Pluto
		gField12->setGConstant(30);
		pPoolObjects->getForceRegistry()->AddRegistry(gField12, earth);


		/// MARS

		GravityField* gField3 = new GravityField(mars, sun, true, 20); // Marte y Sol
		gField3->setGConstant(30);
		pPoolObjects->getForceRegistry()->AddRegistry(gField3, mars);

		GravityField* gField10 = new GravityField(mars, pluto, false, 0); // Marte y Pluto
		gField10->setGConstant(30);
		pPoolObjects->getForceRegistry()->AddRegistry(gField10, mars);

		GravityField* gField4 = new GravityField(mars, earth, false, 0); // Marte y Tierra
		gField4->setGConstant(30);
		pPoolObjects->getForceRegistry()->AddRegistry(gField4, mars);



		/// PLUTO

		GravityField* gField7 = new GravityField(pluto, sun, true, 20); // Pluto y sol
		gField7->setGConstant(30);
		pPoolObjects->getForceRegistry()->AddRegistry(gField7, pluto);

		GravityField* gField9 = new GravityField(pluto, mars, false, 0); // Pluto y Marte
		gField9->setGConstant(30);
		pPoolObjects->getForceRegistry()->AddRegistry(gField9, pluto);

		GravityField* gField13 = new GravityField(pluto, earth, false, 0); // Pluto y Tierra
		gField13->setGConstant(30);
		pPoolObjects->getForceRegistry()->AddRegistry(gField13, pluto);


		/// SUN

		GravityField* gField2 = new GravityField(sun, earth, false, 0); // Sol y Tierra
		pPoolObjects->getForceRegistry()->AddRegistry(gField2, sun);

		GravityField* gField6 = new GravityField(sun, mars, false, 0); // Sol y Marte
		pPoolObjects->getForceRegistry()->AddRegistry(gField6, sun);

		GravityField* gField8 = new GravityField(sun, pluto, false, 0); // Sol y Pluto
		pPoolObjects->getForceRegistry()->AddRegistry(gField8, sun);

		/// STARS
		pSystem = new ParticleSystem();

		SpaceParticleGenerator* stars = new SpaceParticleGenerator(physx::PxVec3(200, 0, 200), physx::PxVec3(0, 0, 0));
		stars->setMeanParticles(2);
		stars->setParticle(new Particle({ 2000, 0, 2000 }, { 0, 0, 0 }, { 0, -.1, 0 }, 1, 0.5, { 1, 1, 0.7, 1 }, 15000, true));

		pSystem->addParticleGenerator(stars);

	}
		break;
	}
}

void Scene::Update(double t)
{
	// Rigids
	pPoolObjects->Update(t);

	// Particles
	if (pSystem != nullptr)
		pSystem->Integrate(t);
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

	//// Register Render Item
	//pPoolObjects->addRenderItem(rigidCont, new RenderItem(shape, particle, color));

	//// Register Shape
	//pPoolObjects->addRigid(rigidCont, particle);

	//rigidCont++;

	//gScene->addActor(*particle);
	return particle;
}

physx::PxRigidDynamic* Scene::createRigidDynamic(const physx::PxVec3& pos, PxMaterial* material, const PxGeometry& geo, const PxVec4& color, double lifeTime)
{
	// Create Rigid Dynamic
	physx::PxRigidDynamic* particle = gPhysics->createRigidDynamic(physx::PxTransform(pos));

	// Material & shape (DEFAULT)
	/*PxMaterial* gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);*/
	physx::PxShape* shape = gPhysics->createShape(geo, *material);
	particle->attachShape(*shape);

	// user data
	RigidInfo* newInfo = new RigidInfo();
	newInfo->actLifeTime = 0;
	newInfo->maxLifeTime = lifeTime;
	newInfo->pStatus = S_ACTIVE;

	particle->userData = (void*) newInfo;

	// Register Render Item
	pPoolObjects->addRenderItem(particle, new RenderItem(shape, particle, color));

	// Test Properties
	// particle->setLinearVelocity({ 0, 10, 0 });
	// particle->setAngularVelocity({ 0, 0, 0 });

	// Add Actor to the scene
	gScene->addActor(*particle);

	return particle;
}

