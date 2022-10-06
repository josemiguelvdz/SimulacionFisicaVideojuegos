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
		AddParticle(new Particle({ 0, 0, 0 }, { 0, 10, 0 }, {0, 0, 0}, 1, 1, {0,0,1,1}, 0, true));
		break;
	case 1:
		// Escena de simulación de proyectiles
		AddPlane(new Plane({ 40, 40, 40 }));

		diana = new Particle({ 40, 50, 40 }, { 0, 0, 0 }, { 0, 0, 0 }, 0, 1, {0, 0, 1, 1}, 0, true);
		AddParticle(diana);
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
		else
			p->integrate(t);

		cont++;
	}
		
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

	mParticles.clear();
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
