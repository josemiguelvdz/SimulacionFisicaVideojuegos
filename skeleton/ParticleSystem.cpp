#include "ParticleSystem.h"


#include <iostream>

ParticleSystem::ParticleSystem()
{
	// basic
	// gForceGenerator = new GravityForceGenerator(physx::PxVec3(0, -0.3, 0));
	// dForceGenerator = new ParticleDragGenerator(0.03, 0);
	// wForceGenerator = new WindForceGenerator(physx::PxVec3(-10, 0, 0));

	// torbellino y explosion
	//tForceGenerator = new TorbellinoForceGenerator(physx::PxVec3(30, -20, 30), 5, 20);
	// eForceGenerator = new ExplosionForceGenerator(physx::PxVec3(30, 50, 30), 50, 70, 0, 100, 2000);

	fw = new FireWorkParticleGenerator();
	fRegistry = new ForceRegistry();

	
	
}

ParticleSystem::~ParticleSystem() {
	for (auto p : mParticles)
		delete p;

	for (auto g : mParticleGenerators)
		delete g;

	for (auto p : mTornadoParticles)
		delete p.first;

	for (auto t : mTornados)
		delete t;

	delete fw;
	fw = nullptr;

	for (auto f : fRegistry->getAllForces())
		delete f;

	mParticles.clear();
	mParticleGenerators.clear();
	mTornadoParticles.clear();
	mTornados.clear();
	fRegistry->clear();
	fRegistry->getAllForces().clear();
}

void ParticleSystem::InitTornados() {
	for (auto g : mParticleGenerators) {
		if (g->getName() == "FUENTE") { // Afectados por torbellino
			TorbellinoForceGenerator* tForce = new TorbellinoForceGenerator(g->getStdPos(), 50, 30);
			mTornados.push_back(tForce);
		}
	}
}

void ParticleSystem::Integrate(double t)
{
	for (auto g : mParticleGenerators) {
		std::list<Particle*> test = g->generateParticles();
		for (auto i : test) {
			if (g->getName() == "FUENTE") { // Afectados por torbellino
				for(auto t : mTornados)
					fRegistry->AddRegistry(t, i);

				mTornadoParticles.push_back(std::pair<Particle*, ParticleGenerator*>(i, g));
			}
			else
				mParticles.push_back(i);
		}
			
	}
	
	std::list<Particle*>::iterator it = mParticles.begin();

	while (it != mParticles.end())
	{
		if (!(*it)->isAlive()) {
			delete *it;
			*it = nullptr;
			it = mParticles.erase(it);
		}
		else {
			if ((*it)->isFirework()) {
				float newScale = (*it)->getScale() - (1.3 * t * (*it)->getIniScale());
				(*it)->setScale(newScale);
			}

			(*it)->integrate(t);

			it++;
		}
			
	}

	std::list<std::pair<Particle*, ParticleGenerator*>>::iterator it2 = mTornadoParticles.begin();

	// std::cout << mTornadoParticles.size() << std::endl;

	// std::cout << mTornadoParticles.size() << std::endl;
	while (it2 != mTornadoParticles.end())
	{
		if (!(*it2).first->isAlive()) {
			(*it2).second->subCont();

			fRegistry->DeleteParticle((*it2).first);

			delete (*it2).first;
			(*it2).first = nullptr;
			it2 = mTornadoParticles.erase(it2);
		}
		else {
			(*it2).first->integrate(t);
			it2++;
		}
	}

	if (fRegistry != nullptr)
		fRegistry->Integrate(t);
}

void ParticleSystem::addParticleGenerator(ParticleGenerator* pGenerator)
{
	mParticleGenerators.push_back(pGenerator);
}

void ParticleSystem::planetExplosion(physx::PxVec3& iniPos, physx::PxVec3& iniVel, float& iniScale)
{
	if (fw != nullptr) {
		std::list<Particle*> exp;
		exp = fw->planetExplosion(iniPos, iniVel, iniScale);

		for (auto it : exp) // Añadimos a particles
			mParticles.push_back(it);
	}
		
}
