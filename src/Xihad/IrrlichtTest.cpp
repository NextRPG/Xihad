#include <irrlicht/irrlicht.h>
#include <irrlicht/IParticleSystemFactory.h>
#include <irrlicht/IParticleEmitter.h>
#include <irrlicht/IParticleRenderer.h>
#include <irrlicht/IPointParticleRenderer.h>
#include <irrlicht/IParticleInitializer.h>
#include <irrlicht/IParticleBoxInitializer.h>
#include <irrlicht/IParticleBasicInitializer.h>
#include <irrlicht/IParticleCompositeInitializer.h>
#include <irrlicht/IParticleForceAffector.h>
#include <irrlicht/IParticleFadeOutAffector.h>
#include <irrlicht/IParticleAffector.h>
#include <irrlicht/IParticleDirectionInitializer.h>
#include <irrlicht/IQuadParticleRenderer.h>
#include <irrlicht/IParticleSphereInitializer.h>
#include <irrlicht/IParticleAttractionAffector.h>
#include "CreateDevice.h"
#include <iostream>

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
int irr_test(int argc, const char** argv)
{
	IrrlichtDevice* device = createDefaultDevice();
	ISceneManager* smgr = device->getSceneManager();
	IVideoDriver* driver = device->getVideoDriver();

	smgr->addCameraSceneNode(0, core::vector3df(0,0,-20));

	IParticleSystemFactory* pfactory = smgr->getParticleSystemFactory();
	IParticleSystemSceneNode* pnode = smgr->addParticleSystemSceneNode(100000, 0, -1);

	IParticleEmitter* emitter = pfactory->createEmitter();
	emitter->setMinParticlesPerSecond(1000);
	emitter->setMaxParticlesPerSecond(2000);
	pnode->addEmitter(emitter);

	// IQuadParticleRenderer* brenderer =  pfactory->createDirectionalRenderer();
	// pnode->setRenderer(brenderer);
	// brenderer->drop();

	IPointParticleRenderer* prenderer = pfactory->createPointParticleRenderer();
	prenderer->setPointSize(3);
	pnode->setRenderer(prenderer);
	prenderer->drop();

	IParticleCompositeInitializer* cinit = pfactory->createCompositeInitializer();

	IParticleSphereInitializer* sphinit = pfactory->createSphereInitializer();
	sphinit->setCenter(core::vector3df());
	sphinit->setRadius(0.f);
	cinit->pushBack(sphinit);
	sphinit->drop();

	IParticleBasicInitializer* basicInit = pfactory->createBasicParticleInitializer();
	basicInit->setMaxLifeTime(30);
	basicInit->setMinLifeTime(20);
	basicInit->setMaxStartColor(video::SColor(255, 200, 64, 64));
	basicInit->setMinStartColor(video::SColor(255, 64, 200, 64));
	basicInit->setMaxStartSize(core::dimension2df(0.1f, 1));
	basicInit->setMinStartSize(core::dimension2df(0.1f, 0.6f));
	cinit->pushBack(basicInit);
	basicInit->drop();

	IParticleDirectionInitializer* dinit = pfactory->createDirectionInitializer();
	dinit->setDirection(core::vector3df(0,6.f,0));
	dinit->setMaxAngleDegree(45);
	cinit->pushBack(dinit);
	dinit->drop();
	
	emitter->setParticleInitializer(cinit);
	emitter->drop();
	cinit->drop();

// 	IParticleForceAffector* gaff = pfactory->createGravityAffector();
// 	gaff->setGravity(core::vector3df(0,-3.f,0));
// 	gaff->setForce(core::vector3df(0.08f, 0, 0));
// 	pnode->addAffector(gaff);
// 	gaff->drop();

	IParticleFadeOutAffector* faff = pfactory->createFadeOutAffector();
	faff->setTargetColor(video::SColor(0, 0, 0, 0));
	pnode->addAffector(faff, 0.7f, 1.0f);
	faff->drop();

	IParticleAffector* saff = pfactory->createScaleAffector();
	pnode->addAffector(saff);
	saff->drop();

	IParticleAttractionAffector* ataff = pfactory->createAttractionAffector();
	ataff->setPoint(core::vector3df(1,1,1));
	ataff->setMass(100);
	pnode->addAffector(ataff);

	pnode->setMaterialFlag(video::EMF_LIGHTING, false);
	pnode->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	pnode->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);

	core::vector3df dpos(0.03f, 0,0);

	ISceneNode* cube = smgr->addCubeSceneNode(1.f);

	int lastFPS = -1;
	while (device->run())
	{
		if (core::abs_(ataff->getPoint().X) > 4)
			dpos = -dpos;
		
		ataff->setPoint(ataff->getPoint() + dpos);
		cube->setPosition(ataff->getPoint());

		driver->beginScene(true, true, SColor(255, 144, 100, 122));
		smgr->drawAll();
		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw tmp(L"Particle System Stress Test [");
			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			tmp += L" particles: ";
			tmp += pnode->getParticleCount();

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
	}

	ataff->drop();
	return 0;
}
