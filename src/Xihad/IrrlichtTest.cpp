#include <irrlicht/irrlicht.h>
#include <irrlicht/IParticleSystemFactory.h>
#include <irrlicht/IParticleEmitter.h>
#include <irrlicht/IParticleRenderer.h>
#include <irrlicht/IParticleInitializer.h>
#include <irrlicht/IParticleBoxInitializer.h>
#include <irrlicht/IBasicParticleInitializer.h>
#include <irrlicht/IParticleCompositeInitializer.h>
#include <irrlicht/IParticleGravityAffector.h>
#include <irrlicht/IParticleFadeOutAffector.h>
#include <irrlicht/IParticleDirectionInitializer.h>
#include <irrlicht/IQuadParticleRenderer.h>
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
	IParticleSystemSceneNode* pnode = smgr->addParticleSystemSceneNode(
		true, 1000, 0, -1, core::vector3df(0, -5, 0));

	pnode->getEmitter()->setMaxParticlesPerSecond(100);

	IParticleRenderer* brenderer =  pfactory->createBillboardRenderer();
	pnode->setRenderer(brenderer);
	brenderer->drop();

	IParticleBoxInitializer* boxinit = pfactory->createBoxInitializer();
	boxinit->setBox(core::aabbox3df(core::vector3df(2, 2, 2)));

	IBasicParticleInitializer* basicInit = pfactory->createBasicParticleInitializer();
	basicInit->setMaxLifeTime(3000);
	basicInit->setMinLifeTime(2000);
	basicInit->setMaxStartColor(video::SColor(255, 200, 64, 64));
	basicInit->setMinStartColor(video::SColor(255, 64, 200, 64));
	basicInit->setMaxStartSize(core::dimension2df(1, 1));
	basicInit->setMinStartSize(core::dimension2df(0.8f, 0.6f));

	IParticleDirectionInitializer* dinit = pfactory->createDirectionInitializer();
	dinit->setDirection(core::vector3df(0,0.006f,0));
	dinit->setMaxAngleDegree(45);

	IParticleCompositeInitializer* cinit = pfactory->createCompositeInitializer();
	cinit->pushBack(boxinit);
	cinit->pushBack(basicInit);
	cinit->pushBack(dinit);

	pnode->getEmitter()->setParticleInitializer(cinit);
	boxinit->drop();
	basicInit->drop();
	cinit->drop();
	dinit->drop();

	IParticleGravityAffector* gaff = pfactory->createGravityAffector();
	gaff->setGravity(core::vector3df(0,-0.003f,0));
	gaff->setTimeForceLost(10000);
	pnode->addAffector(gaff);
	gaff->drop();

	IParticleFadeOutAffector* faff = pfactory->createFadeOutAffector();
	faff->setFadeOutTime(1500);
	faff->setTargetColor(video::SColor(0, 0, 0, 0));
	pnode->addAffector(faff, 0.7f, 1.0f);
	faff->drop();

	pfactory->createScaleAffector(core::dimension2df(5,5));

	pnode->setMaterialFlag(video::EMF_LIGHTING, false);
	pnode->setMaterialFlag(video::EMF_ZWRITE_ENABLE, false);
	pnode->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	while (device->run())
	{
		driver->beginScene(true, true, SColor(255, 144, 100, 122));
		smgr->drawAll();
		driver->endScene();
	}

	return 0;
}
