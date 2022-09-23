#pragma once

#include "Particle.h"
#include "Projectile.h"
#include "Plane.h"
#include <vector>

using namespace std;

class Scene
{
	int mID = 0;
	vector<Particle*> mParticles;
	vector<Projectile*> mProjectiles;
	vector<Plane*> mPlanes;

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
};

