#include "Scene.h"

#include <iostream>

Scene::Scene()
{
	LoadScene(0);
}

Scene::~Scene()
{
	ClearScene();
}

void Scene::LoadScene(int newID)
{
	ClearScene();

	mID = newID;

	Particle* diana = nullptr;

	switch (mID) {
	case 0:
		AddParticle(new Particle({ 0, 0, 0 }, { 0, 10, 0 }, { 0, 0, 0 }, 1, 1, { 0,0,1,1 }, 0, true));
		break;
	case 1:
		// Escena de simulación de proyectiles
		AddPlane(new Plane({ 40, 40, 40 }));

		diana = new Particle({ 40, 50, 40 }, { 0, 0, 0 }, { 0, 0, 0 }, 0, 1, { 0, 0, 1, 1 }, 0, true);
		AddParticle(diana);
		break;
	case 2:
	{
		AddPlane(new Plane({ 40, 40, 40 }));

		pSystem = new ParticleSystem();
		GaussianParticleGenerator* gauss = new GaussianParticleGenerator(physx::PxVec3(30, 40, 30), physx::PxVec3(0, 50, 0), 0, 1);
		gauss->setMeanParticles(1);

		Particle* prefab = new Particle({ 30, 40, 30 }, { 0, 0, 0 }, { 0, 0, 0 }, 1, 0.6, { 0,0,1,1 }, 2000, false);

		prefab->setMass(1);
		prefab->setIMass((float)1 / prefab->getMass());

		gauss->setParticle(prefab);

		pSystem->addParticleGenerator(gauss);
		break;
	}
	case 3:
	{
		AddPlane(new Plane({ 40, 40, 40 }));

		pSystem = new ParticleSystem();
		UniformParticleGenerator* uniform = new UniformParticleGenerator(physx::PxVec3(30, 40, 30), physx::PxVec3(0, 50, 0));
		uniform->setMeanParticles(5);
		uniform->setParticle(new Particle({ 30, 40, 30 }, { 0, 0, 0 }, { 0, -30, 0 }, 1, 0.6, { 0,0,1,1 }, 2000, false));
		pSystem->addParticleGenerator(uniform);

	}
	break;
	case 4:
	{
		AddPlane(new Plane({ 40, 40, 40 }));

		pSystem = new ParticleSystem();
		NieveParticleGenerator* humo = new NieveParticleGenerator(physx::PxVec3(40, 40, 40), physx::PxVec3(5, -3, 5), 0, 1);
		humo->setMeanParticles(10);
		humo->setParticle(new Particle({ 40, 40, 40 }, { 0, 0, 0 }, { 0, -.5, 0 }, 1, 0.5, { 0,0,1,1 }, 10000, false));
		pSystem->addParticleGenerator(humo);
	}
	break;
	case 5:
	{
		// Snippets::backgroundColor = physx::PxVec3(0, 0, 0);
		// AddPlane(new Plane({ 40, 40, 40 }));

		pSystem = new ParticleSystem();
		SpaceParticleGenerator* stars = new SpaceParticleGenerator(physx::PxVec3(40, 40, 40), physx::PxVec3(0, 0, 0));
		stars->setMeanParticles(5);
		stars->setParticle(new Particle({ 40, 40, 40 }, { 0, 0, 0 }, { 0, -.1, 0 }, 1, 0.5, { 1, 1, 0.7, 1 }, 4000, false));

		SpaceParticleGenerator* stars2 = new SpaceParticleGenerator(physx::PxVec3(40, 40, 40), physx::PxVec3(0, 0, 0));
		stars2->setMeanParticles(1);
		stars2->setParticle(new Particle({ 40, 40, 40 }, { 0, 0, 0 }, { 0, -.1, 0 }, 1, 1, { .4 ,0,.8,1 }, 5000, false));

		pSystem->addParticleGenerator(stars);
		pSystem->addParticleGenerator(stars2);

		Particle* planet1 = new Particle({ 3000, 0, 400 }, { 0, 0, 0 }, { 0, 0, 0 }, 1, 300, { 1 ,.6, 0, 1 }, 0, true);
		planet1->setName("Sol");
		AddParticle(planet1);

	}
	case 6:
		fw = new FireWorkParticleGenerator(physx::PxVec3(0, 0, 0), physx::PxVec3(0, 0, 0));
		break;
	case 7:
		generateSpringDemo();
		break;

	case 8:
		generateSlinky();
		break;
	case 9:
		break;
	default:
		break;
	}
	
}

void Scene::Update(double t)
{
	int cont = 0;
	for (auto p : mParticles) {
		if (!p->isAlive()) {
			RemoveParticle(cont);
			cont--;
		}
		else {
			fg.Integrate(t);
			p->integrate(t);

			if(p->mName == "Sol")
				UpdateSun(p);
		}

		cont++;
	}

	

	if(pSystem != nullptr)
		pSystem->Integrate(t);

	if(fw != nullptr)
		fw->integrate(t);

	
		
}

int Scene::AddParticle(Particle* p)
{
	if (p != nullptr) {
		mParticles.push_back(p);
		return mParticles.size() - 1;
	}
	else return -1;
}

int Scene::AddPlane(Plane* p)
{
	if (p != nullptr) {
		mPlanes.push_back(p);
		return mPlanes.size() - 1;
	}
	else return -1;
}

Particle* Scene::GetParticle(int id)
{
	if (id >= 0 && id < mParticles.size())
		return mParticles[id];
	else return nullptr;
}

bool Scene::RemoveParticle(int id)
{
	if (id < 0 && id >= mParticles.size())
		return false;

	vector<Particle*>::iterator it = mParticles.begin();
	int i = 0;
	while (i < id) {
		it++;
		i++;
	}

	delete mParticles[i];
	mParticles.erase(it);
	return true;
}

void Scene::ClearScene()
{
	for (auto p : mParticles)
		delete p;

	for (auto p : mPlanes)
		delete p;

	mParticles.clear();
	mPlanes.clear();

	if (pSystem != nullptr) {
		delete pSystem;
		pSystem = nullptr;
	}

	if (fw != nullptr) {
		delete fw;
		fw = nullptr;
	}
		
}

void Scene::ShootBullet(){
	//Crea el proyectil delante de la camara

	Snippets::Camera* mCamera = GetCamera();
	physx::PxVec3 iniPos = mCamera->getTransform().p;
	physx::PxVec3 iniDir = mCamera->getDir();

	Particle* p = new Particle(iniPos, iniDir, false);
	Projectile real, simulated;

	real.initialHeight = mCamera->getTransform().p.y;

	// Masa real: 0.18 kg
	// Velocidad real: 360 m/s
	real.inverseMass = 1 / .18;
	real.speed = 360;
	real.gravity = mGravity;

	// Damping
	p->setDamp(0.95);

	//Velocidad deseada: 40 m/s
	simulated.speed = 40;
	simulated = Simulate(simulated.speed, real);

	// New vel of the simulated projectile

	p->setVel(iniDir.getNormalized() * simulated.speed);
	p->setAcc({ 0, float(simulated.gravity), 0 });
	p->setIMass(simulated.inverseMass);

	p->setColor({ 1,1,0,1 });
	p->setShape(CreateShape(physx::PxSphereGeometry(0.1f)));

	AddParticle(p);
}

void Scene::ShootHeavyBullet() {
	Snippets::Camera* mCamera = GetCamera();
	physx::PxVec3 iniPos = mCamera->getTransform().p;
	physx::PxVec3 iniDir = mCamera->getDir();

	Particle* p = new Particle(iniPos, iniDir, false);
	Projectile real, simulated;

	real.initialHeight = mCamera->getTransform().p.y;

	// Masa real: 200 kg
	real.inverseMass = 1 / 200.0f;
	real.speed = 10;
	real.gravity = mGravity;

	// Damping
	p->setDamp(0.95);

	simulated.speed = 20;
	simulated = Simulate(simulated.speed, real);

	// New vel of the simulated projectile

	p->setVel(iniDir.getNormalized() * simulated.speed);
	p->setAcc({ 0, float(simulated.gravity), 0 });
	p->setIMass(simulated.inverseMass);

	p->setColor({ 0,1,0,1 });
	p->setShape(CreateShape(physx::PxSphereGeometry(1.5f)));

	AddParticle(p);

}

void Scene::ShootLightBullet() {
	Snippets::Camera* mCamera = GetCamera();
	physx::PxVec3 iniPos = mCamera->getTransform().p;
	physx::PxVec3 iniDir = mCamera->getDir();

	Particle* p = new Particle(iniPos, iniDir, false);
	Projectile real, simulated;

	real.initialHeight = mCamera->getTransform().p.y;

	// Masa real: 200 kg
	real.inverseMass = 1 / .5;
	real.speed = 100;
	real.gravity = mGravity;

	// Damping
	p->setDamp(0.95);

	simulated.speed = 40;
	simulated = Simulate(simulated.speed, real);

	// New vel of the simulated projectile

	p->setVel(iniDir.getNormalized() * simulated.speed);
	p->setAcc({ 0, float(-simulated.gravity), 0 });
	p->setIMass(simulated.inverseMass);

	p->setColor({ 0,.5,.5,1 });
	p->setShape(CreateShape(physx::PxSphereGeometry(1.0f)));


	AddParticle(p);

}

Scene::Projectile Scene::Simulate(double simVel, Projectile rProj)
{
	// Creation of the simulated projectile
	Projectile sProj;

	sProj.speed = simVel;
	sProj.initialHeight = rProj.initialHeight;

	// E = mv
	if (rProj.speed == 0)
		sProj.inverseMass = rProj.inverseMass;
	else
		sProj.inverseMass = rProj.inverseMass * pow(sProj.speed / rProj.speed, 2);

	//Queremos que recorra la misma distancia	0 = p + vt + .5 * at^2
	if (simVel == 0)
		sProj.gravity = rProj.gravity;
	else if (rProj.gravity != 0) {
		const double rFlightTime = sqrt(2 * rProj.initialHeight / abs(rProj.gravity));
		const double distanceReached = rProj.speed * rFlightTime;
		const double sFlightTime = distanceReached / sProj.speed;

		sProj.gravity = rProj.gravity * pow(rFlightTime, 2) / pow(sFlightTime, 2);
	}
	else
		sProj.gravity = 0;

	return sProj;
}


void Scene::CreateFireWork() {
	// cholin si ves esto eres wekereke
	int max = 2;
	int min = 0;
	int x = rand() % (max - min + 1) + min;
	int y = rand() % (max - min + 1) + min;

	Particle* p = new Particle({ 30, 20, 30 }, { (float)x, 10, (float)y }, { 0, 40, 0 }, 1, 0.5, { 1 ,0, 0, 1 }, 1000, false);

	if(fw != nullptr)
		fw->createFireWorkParticle(p);

}

void Scene::UpdateSun(Particle* p) {
	static double suma = 100.0;
	static double color_r = 1;
	static double color_g = .6;
	static double color_b = .2;

	static bool expansion = false;
	static bool contraccion = false;

	p->setShape(CreateShape(physx::PxSphereGeometry(suma)));
	p->setColor(physx::PxVec4(color_r, color_g, color_b, 1));
	// 
	suma += 0.07;

	color_g -= 0.0001;
	if (color_g < 0)
		color_g = 0;
	color_b -= 0.0001;
	if (color_b < 0)
		color_b = 0;

	if (suma >= 600 && !contraccion) {
		contraccion = true;
	}

	if (contraccion && !expansion) {
		suma -= 30;

		color_b += 0.01;
		if (color_b > 1)
			color_b = 1;
		color_r -= 0.01;
		if (color_r < 0)
			color_r = 0;

		if (suma < 10 && !expansion) {
			expansion = true;
		}
	}

	if (expansion) {
		suma += 30;

		color_b += 0.2;
		color_r += 0.2;
		color_g += 0.2;

		if (color_b > 1)
			color_b = 1;
		if (color_g > 1)
			color_g = 1;
		if (color_r > 1)
			color_r = 1;
	}

}

void Scene::generateSpringDemo()
{
	// Unimos 2 particulas
	Particle* p1 = new Particle(physx::PxVec3(0, 50, 0), physx::PxVec3(0, 0, 0), physx::PxVec3(0, 0, 0), 1, 3, physx::PxVec4(1, 0, 0, 1), 1000, true);
	Particle* p2 = new Particle(physx::PxVec3(0, 30, 0), physx::PxVec3(0, 0, 0), physx::PxVec3(0, 0, 0), 0.95, 0.85, physx::PxVec4(0, 1, 0, 1), 1000, true);

	p2->setMass(2.0);
	p2->setIMass((float)1 / p2->getMass());

	p1->setMass(1.0);
	p1->setIMass((float)1 / p1->getMass());

	// Gravedad
	GravityForceGenerator* g1 = new GravityForceGenerator(physx::PxVec3(0, -9.8, 0));
	fg.AddRegistry(g1, p2);

	// Viento
	w1 = new WindForceGenerator(physx::PxVec3(-15, 0, 0));
	w1->setActive(false);
	fg.AddRegistry(w1, p2);


	f1 = new SpringForceGenerator(10, 10, p1);
	fg.AddRegistry(f1, p2);

	mParticles.push_back(p1);
	mParticles.push_back(p2);
}

void Scene::generateSlinky() {
	// Unimos 2 particulas
	Particle* p0 = new Particle(physx::PxVec3(0, 80, 0), physx::PxVec3(0, 0, 0), physx::PxVec3(0, 0, 0), 1, 3, physx::PxVec4(1, 1, 1, 1), 1000, true);

	p0->mName = "Static";

	// Gravedad
	GravityForceGenerator* g1 = new GravityForceGenerator(physx::PxVec3(0, -9.8, 0));


	mParticles.push_back(p0);

	for (int i = 3; i > 0; i--) { // TODO: Force Registry
		Particle* newParticle = new Particle(physx::PxVec3(0, 20 * i, 0), physx::PxVec3(0, 0, 0), physx::PxVec3(0, 0, 0), 1, 0.85, physx::PxVec4(1, 0, 0, 1), 1000, true);
		mParticles.push_back(newParticle);
		newParticle->setMass(1.0);
		newParticle->setIMass((float)1 / newParticle->getMass());
		SpringForceGenerator* force = new SpringForceGenerator(1, 20, p0);
		vForceGenerators.push_back(force);
		SpringForceGenerator* force2 = new SpringForceGenerator(1, 20, newParticle);
		vForceGenerators.push_back(force2);

		p0 = newParticle;
	}

	//// Viento
	//w1 = new WindForceGenerator(physx::PxVec3(-15, 0, 0));

	/*f1 = new SpringForceGenerator(10, 10, p1);
	SpringForceGenerator* f2 = new SpringForceGenerator(10, 10, p2);*/

	
	// SlinkyForceGenerator* s1 = new SlinkyForceGenerator(1, 1, p0);
	//SlinkyForceGenerator* s2 = new SlinkyForceGenerator(1, 20, p2);

	//SlinkyForceGenerator* s3 = new SlinkyForceGenerator(1, 10, p3);
	//SlinkyForceGenerator* s4 = new SlinkyForceGenerator(1, 5, p4);

	// vForceGenerators.push_back(f1);
	//vForceGenerators.push_back(g1);
	//vForceGenerators.push_back(s1);
	//vForceGenerators.push_back(s2);
	//vForceGenerators.push_back(s3);
	vForceGenerators.push_back(g1);



}

