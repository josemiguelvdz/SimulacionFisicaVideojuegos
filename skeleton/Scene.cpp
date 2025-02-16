#include "Scene.h"
#include <iostream>

Scene::Scene(PxDefaultCpuDispatcher* dispatcher, PxPhysics* physics, Camera* camera)
{
	gDispatcher = dispatcher;
	gPhysics = physics;
	mCamera = camera;

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
	if (pPoolObjects != nullptr) {
		pPoolObjects->clearScene();
		delete pPoolObjects;
	}

	if (pSystem != nullptr) {
		delete pSystem;
		pSystem = nullptr;
	}

	// Bools
	mZoomActivated = false;
	mGoToCamera = false;

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, 0.0f, 0.0f);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);


	mID = newID;
	pPoolObjects = new PoolObjects();
	pPoolObjects->setActiveScene(gScene);

	switch (mID) {
	case 0:
	{
		mCamera = new Camera(PxVec3(0.0f, 150.0f, 0.0f), PxVec3(0.02f, -0.99f, 0.02f));
		SetCamera(mCamera);

		/*AddParticle(new Particle({ 0, 0, 0 }, { 0, 10, 0 }, { 0, 0, 0 }, 1, 1, { 0,0,1,1 }, 0, true));*/

		// 232, 127, 35
		PxRigidDynamic* sun = createRigidDynamic({ 0, 0, 0 }, gPhysics->createMaterial(0.0f, 0.0f, 0.0f), PxSphereGeometry(5), { (float)232/255, (float)127 / 255, (float)35 / 255, 1}, INT_MAX, 5);
		sun->setAngularDamping(0);
		sun->setAngularVelocity({ 0, 1, 0 });
		//sun->addTorque({ 0.3, 1, -0.3 });

		sun->setMass(200);
		sun->setLinearVelocity({0,0,0});
		sun->setLinearDamping(0);

		sun->setName("Sun");

		PxRigidDynamic* earth = createRigidDynamic({ 55, 0, 40 }, gPhysics->createMaterial(0.0f, 0.0f, 0.0f), PxSphereGeometry(1.7f), { 0, 0, 1, 1 }, INT_MAX, 1.7f);
		earth->setAngularDamping(0);
		earth->setAngularVelocity({ 0, -2 * 3.14, 0 });
		earth->setMass(10);

		earth->setName("Earth");

		earth->setLinearDamping(0);

		PxRigidDynamic* mars = createRigidDynamic({ 50, 0, -40 }, gPhysics->createMaterial(0.0f, 0.0f, 0.0f), PxSphereGeometry(0.8f), { 1, 0, 0, 1 }, INT_MAX, 0.8f);
		mars->setAngularDamping(0);
		mars->setAngularVelocity({ 0, -2 * 3.14, 0 });
		mars->setMass(0.2);

		mars->setLinearDamping(0);

		PxRigidDynamic* pluto = createRigidDynamic({ 5, 0, 10 }, gPhysics->createMaterial(0.0f, 0.0f, 0.0f), PxSphereGeometry(0.6f), { 0, 1, 0, 1 }, INT_MAX, 0.6f);
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

		SpaceParticleGenerator* stars = new SpaceParticleGenerator(physx::PxVec3(200, -500, 200), physx::PxVec3(0, 0, 0));
		stars->setMeanParticles(2);
		stars->setParticle(new Particle({ 2000, 0, 2000 }, { 0, 0, 0 }, { 0, -.1, 0 }, 1, 0.5, { 1, 1, 0.7, 1 }, 15000, true));

		pSystem->addParticleGenerator(stars);
	}
		break;

	case 1:
		
		/*AddParticle(new Particle({ 0, 0, 0 }, { 0, 10, 0 }, { 0, 0, 0 }, 1, 1, { 0,0,1,1 }, 0, true));*/
	{
		mCamera = new Camera(PxVec3(0.0f, 150.0f, 0.0f), PxVec3(0.02f, -0.99f, 0.02f));
		SetCamera(mCamera);

		PxRigidDynamic * sun = createRigidDynamic({ 0, 0, 0 }, gPhysics->createMaterial(0.0f, 0.0f, 0.0f), PxSphereGeometry(5.f), { (float)232 / 255, (float)127 / 255, (float)35 / 255, 1 }, INT_MAX, 5.f);
		sun->setAngularDamping(0);
		sun->setAngularVelocity({ 0, 1, 0 });
		//sun->addTorque({ 0.3, 1, -0.3 });

		sun->setMass(220);
		sun->setLinearVelocity({ 0,0,0 });
		sun->setLinearDamping(0);

		sun->setName("Sun");

		PxRigidDynamic* earth = createRigidDynamic({ 30, 0, 40 }, gPhysics->createMaterial(0.0f, 0.0f, 0.0f), PxSphereGeometry(1.3f), { 0, 0, 1, 1 }, INT_MAX, 1.3f);
		earth->setAngularDamping(0);
		earth->setAngularVelocity({ 0, -2 * 3.14, 0 });
		earth->setMass(10);

		earth->setName("Earth");

		earth->setLinearDamping(0);

		PxRigidDynamic* mars = createRigidDynamic({ -10, 0, -15 }, gPhysics->createMaterial(0.0f, 0.0f, 0.0f), PxSphereGeometry(0.8f), { 1, 0, 0, 1 }, INT_MAX, 0.8f);
		mars->setAngularDamping(0);
		mars->setAngularVelocity({ 0, -2 * 3.14, 0 });
		mars->setMass(0.2);

		mars->setLinearDamping(0);

		PxRigidDynamic* pluto = createRigidDynamic({ 10, 0, 15 }, gPhysics->createMaterial(0.0f, 0.0f, 0.0f), PxSphereGeometry(0.6f), { 0, 1, 0, 1 }, INT_MAX, 0.6f);
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

		SpaceParticleGenerator* stars = new SpaceParticleGenerator(physx::PxVec3(200, -500, 200), physx::PxVec3(0, 0, 0));
		stars->setMeanParticles(2);
		stars->setParticle(new Particle({ 2000, 0, 2000 }, { 0, 0, 0 }, { 0, -.1, 0 }, 1, 0.5, { 1, 1, 0.7, 1 }, 1.f, true));

		pSystem->addParticleGenerator(stars);

	}
		break;

	case 2:
	{
		mCamera = new Camera({ 50, 60, 50 }, { -0.6f, -0.2f, -0.7f });
		SetCamera(mCamera);
		generateWaterPlanet();
	}
		break;
	case 3:
	{
		mCamera = new Camera({ -100, 40, -400 }, { 0.2f, -0.1f, 0.6f });
		SetCamera(mCamera);
		generateWaterPlanet();
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

	if (mZoomActivated) { // Hacer zoom
		// Aumentar Escala
		PxRigidDynamic* earth = pPoolObjects->getEarth();
		if (earth != nullptr) {
			if (!mGoToCamera) {
				PxTransform newPos(lerp(earth->getGlobalPose().p, { 0, 20, 0 }, t), earth->getGlobalPose().q);
				earth->setGlobalPose(newPos);
			}

			if (PxVec3{ 0, 20, 0 }.y - earth->getGlobalPose().p.y < PxVec3{0, 0.5, 0}.y)
				mGoToCamera = true;

			if (mGoToCamera) {
				PxTransform newPos(lerp(earth->getGlobalPose().p, GetCamera()->getTransform().p, t), earth->getGlobalPose().q);
				earth->setGlobalPose(newPos);

				if (GetCamera()->getTransform().p.y - earth->getGlobalPose().p.y < PxVec3{ 0, 1, 0 }.y) {
					// Cargar nueva escena
					LoadScene(2);
				}
					
			}
		}
	}
}

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

physx::PxRigidDynamic* Scene::createRigidDynamic(const physx::PxVec3& pos, PxMaterial* material, const PxGeometry& geo, const PxVec4& color, double lifeTime, float scale)
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
	newInfo->scale = scale;
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

void Scene::generateWaterPlanet()
{
	// Gravedad
	gScene->setGravity({ 0, -20, 0 });

	// Water
	Particle* water = new Particle(physx::PxVec3(30, 40, 30), physx::PxVec3(0, 0, 0), physx::PxVec3(0, 0, 0), 1, 1, physx::PxVec4(0, 0, 1, 1), 1.f, true);
	water->setShape(CreateShape(physx::PxBoxGeometry(1000, 0.01, 1000)));

	Particle* waterDec = new Particle(physx::PxVec3(30, -100, 30), physx::PxVec3(0, 0, 0), physx::PxVec3(0, 0, 0), 1, 1, physx::PxVec4(0, 0, 1, 1), 1.f, true);
	waterDec->setShape(CreateShape(physx::PxBoxGeometry(10000, 3, 10000)));


	// Particle System
	pSystem = new ParticleSystem();
	pSystem->addParticle(waterDec);
	pSystem->addParticle(water);

	// Cubo Test
	Particle* cube = new Particle(physx::PxVec3(30, 60, 30), physx::PxVec3(0, 0, 0), physx::PxVec3(0, 0, 0), 0.5, 1, physx::PxVec4(1, 0, 0, 1), 20.f, false);
	cube->setShape(CreateShape(physx::PxBoxGeometry(1, 1, 1)));
	cube->setMass(1);
	cube->setIMass((float)1 / cube->getMass());

	pSystem->addParticle(cube);

	// Forces Particles
	gf = new GravityForceGenerator(physx::PxVec3(0, -10, 0));
	pSystem->getForceRegistry()->addForceToData(gf);
	pSystem->getForceRegistry()->AddRegistry(gf, cube);
	w = new WindForceGenerator(physx::PxVec3(-5, 0, -5));
	pSystem->getForceRegistry()->addForceToData(w);
	pSystem->getForceRegistry()->AddRegistry(w, cube);
	b = new BuoyancyForceGenerator(water, 0.1, 5000, pow(0.1, 3));
	pSystem->getForceRegistry()->addForceToData(b);
	pSystem->getForceRegistry()->AddRegistry(b, cube);


	PxRigidDynamic* test = createRigidDynamic({ -80, 60, -360 }, gPhysics->createMaterial(0.f, 0.f, 0.f), PxSphereGeometry(2), { 1, 0, 0, 1 }, 20.f, 2);
	test->setLinearDamping(0.8);
	test->setMass(3);

	test->setWindFriction(1);
	test->setWindFriction2(0);


	// Forces Rigids
	wRigids = new RigidWindFGenerator(physx::PxVec3(10, 0, 30));
	pPoolObjects->getForceRegistry()->addForceToData(wRigids);
	pPoolObjects->getForceRegistry()->AddRegistry(wRigids, test);

	bRigids = new RigidBuoyancyFGenerator(water, 0.1, 10000, pow(0.1, 3));
	pPoolObjects->getForceRegistry()->addForceToData(bRigids);
	pPoolObjects->getForceRegistry()->AddRegistry(bRigids, test);

	tRigids = new RigidTorbellinoFGenerator({ -40, 40, -250 }, 30, 100);
	pPoolObjects->getForceRegistry()->addForceToData(tRigids);
	tRigids->invertTornado();
	pPoolObjects->getForceRegistry()->AddRegistry(tRigids, test);

	// Tornados
	generateTornados({ -40, 40, -40 }, 1);
	generateTornados({ -40, 40, -250 }, -1);
	//generateTornados({ -200, -20, -150 });

	pSystem->InitTornados();

	pSystem->addCubeToTornado(cube);

	/// STARS

	SpaceParticleGenerator* stars = new SpaceParticleGenerator(physx::PxVec3(100, 400, 100), physx::PxVec3(0, 0, 0));
	stars->setMeanParticles(3);
	stars->setParticle(new Particle({ 2000, 0, 2000 }, { 0, 0, 0 }, { 0, -.1, 0 }, 1, 0.5, { 1, 1, 0.7, 1 }, 1, true));

	pSystem->addParticleGenerator(stars);
}

void Scene::generateTornados(physx::PxVec3 tornadoPos, int sentido)
{
	// Tornados

	UniformParticleGenerator* uniform = new UniformParticleGenerator(tornadoPos, physx::PxVec3(0, 20 * sentido, 0));
	uniform->setName("FUENTE");

	uniform->setMeanParticles(1);
	Particle* prefab = new Particle({ -40, -20, -40 }, { 0, 0, 0 }, { 0, 0, 0 }, 1, 0.6, { 0,0,1,1 }, 4.5f, true);
	uniform->setParticle(prefab);
	pSystem->addParticle(prefab);

	pSystem->addParticleGenerator(uniform);

	// Tornado 2
	physx::PxVec3 iniPos;
	if (sentido < 0)
		iniPos = physx::PxVec3(tornadoPos.x, tornadoPos.y, tornadoPos.z);
	else
		iniPos = physx::PxVec3(tornadoPos.x, tornadoPos.y + 20, tornadoPos.z);

	UniformParticleGenerator* uniform2 = new UniformParticleGenerator(iniPos, physx::PxVec3(0, 15 * sentido, 0));
	uniform2->setName("FUENTE");

	uniform2->setMeanParticles(1);
	Particle* prefab2 = new Particle({ -40, 50, -40 }, { 0, 0, 0 }, { 0, 0, 0 }, 1, 0.6, { 0,0,1,1 }, 4.f, true);
	uniform2->setParticle(prefab2);
	pSystem->addParticle(prefab2);

	pSystem->addParticleGenerator(uniform2); 

	// Tornado 3
}

void Scene::generateCube()
{
	// Generar un cubo 
	int posX = std::rand() % (50 - 10 + 1) + 10;
	int posY = std::rand() % (70 - 55 + 1) + 55;
	int posZ = std::rand() % (50 - 10 + 1) + 10;

	int mass = std::rand() % (10 - 1 + 1) + 1;

	Particle* cube = new Particle(physx::PxVec3(posX, posY, posZ), physx::PxVec3(0, 0, 0), physx::PxVec3(0, 0, 0), 0.8, 1, physx::PxVec4(1, 0, 0, 1), 20.f, false);
	cube->setShape(CreateShape(physx::PxBoxGeometry(1, 1, 1)));
	cube->setMass(mass);
	cube->setIMass((float)1 / mass);

	pSystem->addParticle(cube);

	// Forces
	pSystem->getForceRegistry()->AddRegistry(gf, cube);
	pSystem->getForceRegistry()->AddRegistry(w, cube);
	pSystem->getForceRegistry()->AddRegistry(b, cube);


	pSystem->addCubeToTornado(cube);

}

void Scene::generateRandomRigid()
{
	// Generar un solido rigido aleatorio -80, 60, -360
	int posX = std::rand() % (-80 - -60 + 1) + -60;
	int posY = std::rand() % (60 - 45 + 1) + 45;
	int posZ = std::rand() % (-360 - -340 + 1) + -340;

	int mass = std::rand() % (5 - 1 + 1) + 1;

	int sFriction = std::rand() % (10 - 1 + 1) + 1;
	int dFriction = std::rand() % (10 - 1 + 1) + 1;
	int restitution = std::rand() % (10 - 1 + 1) + 1;

	PxRigidDynamic* randomRigid = nullptr;

	if (std::rand() % 2 == 0) { // Circulo
		int rad = std::rand() % (3 - 1 + 1) + 1;

		randomRigid = createRigidDynamic({ (float)posX, (float)posY, (float)posZ }, gPhysics->createMaterial(sFriction, dFriction, restitution), PxSphereGeometry(rad), 
			{ (float)sFriction / 10.f,  (float)dFriction / 10.f,  (float)restitution / 10.f , 1}, 20.f, rad);
		randomRigid->setMass(mass);
		randomRigid->setLinearDamping(0.8);
	}
	else { // Cuadrado
		int boxX = std::rand() % (3 - 1 + 1) + 1;
		int boxY = std::rand() % (3 - 1 + 1) + 1;
		int boxZ = std::rand() % (3 - 1 + 1) + 1;

		randomRigid = createRigidDynamic({ (float)posX, (float)posY, (float)posZ }, gPhysics->createMaterial(sFriction, dFriction, restitution), PxBoxGeometry(boxX, boxY, boxZ),
			{ (float)sFriction / 10.f, (float)dFriction / 10.f,  (float)restitution / 10.f, 1}, 20.f, boxX);
		randomRigid->setMass(mass);
		randomRigid->setLinearDamping(0.8);
	}

	randomRigid->setWindFriction(1);
	randomRigid->setWindFriction2(0);

	// Forces
	pPoolObjects->getForceRegistry()->AddRegistry(wRigids, randomRigid);
	pPoolObjects->getForceRegistry()->AddRegistry(bRigids, randomRigid);
	pPoolObjects->getForceRegistry()->AddRegistry(tRigids, randomRigid);

	// pSystem->addCubeToTornado(cube);
}

void Scene::ZoomToEarth()
{
	// Meter un zoom a la tierra
	mZoomActivated = true;
	pPoolObjects->clearForcesInAllRigids();
	
}

