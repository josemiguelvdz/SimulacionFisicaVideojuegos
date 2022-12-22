#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>
#include <string>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"

#include <iostream>
#include <iomanip>
#include <sstream>

// Scene
#include "Scene.h"


using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation*			gFoundation = NULL;
PxPhysics*				gPhysics	= NULL;


PxMaterial*				gMaterial	= NULL;

PxPvd*                  gPvd        = NULL;

PxDefaultCpuDispatcher*	gDispatcher = NULL;
PxScene*				gScene      = NULL;
ContactReportCallback gContactReportCallback;

Scene* mSM = nullptr;

// FPS
clock_t startTime = -1;
double fps;

void updateWindowName(clock_t endTime) {
	clock_t deltaTime = endTime - startTime;

	fps++;
	// Display every second
	if (deltaTime > 1000) {
		fps--;

		std::stringstream stream;
		stream << std::fixed << std::setprecision(2) << fps;
		std::string s = stream.str();

		std::string name1 = "Simulacion Fisica Videojuegos (FPS: " + s + ")";

		glutSetWindowTitle(name1.c_str());

		fps = 0;
		startTime = endTime;
	}
}


// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport,PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	gDispatcher = PxDefaultCpuDispatcherCreate(2);

	mSM = new Scene(gDispatcher, gPhysics, GetCamera());
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	if (startTime == -1)
		startTime = clock();

	PX_UNUSED(interactive);

	mSM->Update(t);

	clock_t endTime = clock();

	// std::cout << "begin: " << beginFrame << " /// last: " << endFrame << "\n";

	updateWindowName(endTime);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	mSM->getActiveScene()->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();	
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();
	
	gFoundation->release();

	// mSM->ClearScene();
	}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);

	toupper(key);

	if (key >= 48 && key <= 57) {
		mSM->LoadScene((int)key - 48);
	}
	else {
		if (key == 43) { // '+'
			// Aumentar masa SOL
			PxRigidDynamic* sun = mSM->getPoolObjects()->getSun();
			if (sun != nullptr)
				sun->setMass(sun->getMass() + 20);
		}
		if (key == 45) { // '-'
			// Disminuir masa SOL
			PxRigidDynamic* sun = mSM->getPoolObjects()->getSun();
			if (sun != nullptr)
				sun->setMass(sun->getMass() - 20);
		}
		if (key == 'z' && mSM->getID() < 2) // Test Zoom
			mSM->ZoomToEarth();
		if (key == 'c' && mSM->getID() >= 2) // Generar Cubo en Tornado
			mSM->generateCube();
		if (key == 'r' && mSM->getID() >= 2) // Generar Solido Rigido en Torbellino hacia abajo
			mSM->generateRandomRigid();
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	// Si la escena del espacio está activa y además, está vivo
	if (mSM->getID() < 2 && ((RigidInfo*)actor1->userData)->pStatus == S_ACTIVE && ((RigidInfo*)actor2->userData)->pStatus == S_ACTIVE) {
		RigidInfo* newInfo = new RigidInfo();
		
		float m1 = ((PxRigidDynamic*)actor1)->getMass();
		float m2 = ((PxRigidDynamic*)actor2)->getMass();
		if (m1 > m2) {
			// sobrevive body1
			PxRigidDynamic* body2 = (PxRigidDynamic*)actor2;
			float b2Scale = ((RigidInfo*)body2->userData)->scale;

			newInfo->pStatus = S_DEAD;
			body2->userData = newInfo;

			// Aumentar Escala
			PxRigidDynamic* body1 = (PxRigidDynamic*)actor1;
			((RigidInfo*)body1->userData)->scale += b2Scale;

			RenderItem* i = mSM->getPoolObjects()->getRenderItem(body1);
			i->shape = CreateShape(PxSphereGeometry(((RigidInfo*)body1->userData)->scale));

			// Explode
			mSM->getPSystem()->planetExplosion(body2->getGlobalPose().p, body2->getLinearVelocity(), b2Scale);
		}
		else {
			// sobrevive body2
			PxRigidDynamic* body1 = (PxRigidDynamic*)actor1;
			float b1Scale = ((RigidInfo*)body1->userData)->scale;

			newInfo->pStatus = S_DEAD;
			body1->userData = newInfo;

			// Aumentar Escala
			PxRigidDynamic* body2 = (PxRigidDynamic*)actor2;
			((RigidInfo*)body2->userData)->scale += b1Scale;

			RenderItem* i = mSM->getPoolObjects()->getRenderItem(body2);
			i->shape = CreateShape(PxSphereGeometry(((RigidInfo*)body2->userData)->scale));

			// Explode
			mSM->getPSystem()->planetExplosion(body1->getGlobalPose().p, body1->getLinearVelocity(), b1Scale);
		}
	}
	

	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}



int main(int, const char*const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for(PxU32 i=0; i<frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}