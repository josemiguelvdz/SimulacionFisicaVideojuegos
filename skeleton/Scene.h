#pragma once

#include "Particle.h"
#include "Plane.h"
#include "ParticleSystem.h"
#include "GaussianParticleGenerator.h"
#include "UniformParticleGenerator.h"
#include "NieveParticleGenerator.h"
#include "SpaceParticleGenerator.h"
#include "FireWorkParticleGenerator.h"

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
	
};

