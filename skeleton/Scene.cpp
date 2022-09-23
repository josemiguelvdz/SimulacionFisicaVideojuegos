#include "Scene.h"

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

	switch (mID) {
	case 0:
		AddParticle(new Particle({ 0, 0, 0 }, { 0, 10, 0 }, {0, 0, 0}, 1));
		break;
	case 1:
		AddPlane(new Plane({ 0, 0, 0 }));
		break;
	default:
		break;
	}
}

void Scene::Update(double t)
{
	for (auto p : mParticles)
		p->integrate(t);
}

int Scene::AddParticle(Particle* p)
{
	if (p != nullptr) {
		mParticles.push_back(p);
		return mParticles.size() - 1;
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
	
	mParticles.erase(it);
	return true;
}

void Scene::ClearScene()
{
	for (auto p : mParticles)
		delete p;

	mParticles.clear();
}

int Scene::AddPlane(Plane* p)
{
	if (p != nullptr) {
		mPlanes.push_back(p);
		return mPlanes.size() - 1;
	}
	else return -1;
}
