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
		AddPlane(new Plane({ 0, 0, 0 }));

		diana = new Particle({ -100, 50, -100 }, { 0, 0, 0 }, { 0, 0, 0 }, 0, 10, {0, 0, 1, 1}, 0, true);
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
	real.angle = 0;

	// Masa real: 0.18 kg
	// Velocidad real: 360 m/s
	real.inverseMass = 1 / .18;
	real.speed = 360;
	real.gravity = mGravity;
	// real.variation = .1;

	// Damping
	p->setDamp(1);

	//Velocidad deseada: 40 m/s
	simulated.speed = 400;

	simulated = Simulate(simulated.speed, real);

	//Se lanza hacia el frente con una varianza
	/*PxVec3 dir = PxVec3(PxCos(simulated.angle) * simulated.speed * front
		+ PxSin(simulated.angle) * simulated.speed * up);

	if (simulated.variation != 0) {
		normal_distribution<> normal(0, simulated.variation);
		dir = PxVec3(dir.x + normal(generator), dir.y + normal(generator), dir.z + normal(generator));
	}*/

	// New vel of the simulated projectile

	p->setVel(iniDir.getNormalized() * simulated.speed);
	p->setAcc({ 0, float(simulated.gravity), 0 });
	p->setIMass(simulated.inverseMass);

	p->setColor({ 1,1,0,1 });
	p->setShape(CreateShape(physx::PxSphereGeometry(10.0)));

	std::cout << "Masa real: " << 1 / real.inverseMass << " kg	Velocidad real: " << real.speed << " m/s	Gravedad real: " << real.gravity << " m/s^2\n"
		<< "Masa simulada: " << 1 / simulated.inverseMass << " kg	Velocidad simulada: " << simulated.speed << " m/s	Gravedad simulada: " << simulated.gravity << " m/s^2\n\n";

	AddParticle(p);

	// int iniVel = 100;
	// AddParticle(new Particle(iniPos, iniDir.getNormalized() * iniVel, {0, -3, 0}, 1, 3, {1, 1, 0, 1}, 5000, false));

}

void Scene::ShootHeavyBullet() {
	Snippets::Camera* mCamera = GetCamera();
	physx::PxVec3 iniPos = mCamera->getTransform().p;
	physx::PxVec3 iniDir = mCamera->getDir();

	int iniVel = 80;

	physx::PxVec3 test = iniDir.getNormalized();

	AddParticle(new Particle(iniPos, iniDir.getNormalized() * iniVel, { 0, -130, 0 }, 1, 10, { 1, 1, 1, 1 }, 5000, false));

}

void Scene::ShootLightBullet() {
	Snippets::Camera* mCamera = GetCamera();
	physx::PxVec3 iniPos = mCamera->getTransform().p;
	physx::PxVec3 iniDir = mCamera->getDir();

	int iniVel = 100;

	physx::PxVec3 test = iniDir.getNormalized();

	AddParticle(new Particle(iniPos, iniDir.getNormalized() * iniVel, { 0, 5, 0 }, 1, 2, { .5, 0, 1, 1 }, 5000, false));

}

Scene::Projectile Scene::Simulate(double simVel, Projectile rProj)
{
	// Creation of the simulated projectile
	Projectile sProj;

	sProj.speed = simVel;
	sProj.initialHeight = rProj.initialHeight;
	sProj.angle = rProj.angle;
	// sProj.variation = rProj.variation;

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
