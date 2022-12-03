#pragma once

#include "Particle.h"
#include "Plane.h"
#include "ParticleSystem.h"
#include "GaussianParticleGenerator.h"
#include "UniformParticleGenerator.h"
#include "NieveParticleGenerator.h"
#include "SpaceParticleGenerator.h"
#include "FireWorkParticleGenerator.h"

#include "Forces/ForceRegistry.h"



#include <vector>

using namespace std;

class Scene
{
public:
	Scene();
	~Scene();

	void LoadScene(int newID);
	void Update(double t);

	int AddParticle(Particle* p);
	int AddPlane(Plane* p);
	Particle* GetParticle(int id);
	bool RemoveParticle(int id);
	void ClearScene();

	void ShootBullet();
	void ShootHeavyBullet();
	void ShootLightBullet();

	void CreateFireWork();

	void UpdateSun(Particle* p);

	void generateSpringDemo();
	void generateSlinky();
	void generateRuberband();
	void generateWater();

	void addSpringCoef() { f1->setK(f1->getK() + 1); };
	void subSpringCoef() { f1->setK(f1->getK() - 1); };

	void addViento() {
		if (w1 != nullptr)
			w1->setActive(true);
	}

	void subViento() {
		if(w1 != nullptr)
			w1->setActive(false);
	}

	void MovePlayerUp() {
		if (player != nullptr) {
			player->setPos(player->getPos() + physx::PxVec3(0, 2, 0));
		}
	}
	void MovePlayerDown() {
		if (player != nullptr) {
			player->setPos(player->getPos() + physx::PxVec3(0, -2, 0));
		}
	}
	void MovePlayerLeft() {
		if (player != nullptr) {
			player->setPos(player->getPos() + physx::PxVec3(-2, 0, 0));
		}
	}
	void MovePlayerRight() {
		if (player != nullptr) {
			player->setPos(player->getPos() + physx::PxVec3(2, 0, 0));
		}
	}

	BuoyancyForceGenerator* getBuoyancyForceGen() { return b; };

private:

	int mID = 0;
	double mGravity = -9.8;
	vector<Particle*> mParticles;
	vector<Plane*> mPlanes;

	struct Projectile {
		double inverseMass; //inversa de la masa
		double speed; //velocidad
		double gravity; //gravedad
		double initialHeight; //altura inicial
	};


	Projectile Simulate(double simulatedVel, Projectile real);

	ParticleSystem* pSystem = nullptr;
	FireWorkParticleGenerator* fw = nullptr;

	vector<ForceGenerator*> vForceGenerators;

	SpringForceGenerator* f1 = nullptr;
	WindForceGenerator* w1 = nullptr;

	ForceRegistry fg;

	Particle* player = nullptr;
	
	BuoyancyForceGenerator* b = nullptr;


};

