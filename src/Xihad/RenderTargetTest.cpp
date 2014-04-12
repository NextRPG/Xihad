#include <irrlicht/irrlicht.h>
#include "CreateDevice.h"

using namespace irr;
int rt_test(int argc, const char** argv)
{
	IrrlichtDevice* device = //createDefaultDevice();
		createDevice(video::EDT_OPENGL,
			core::dimension2du(1024,600), 32, false,
			false, false, 0);


	video::IVideoDriver* driver = device->getVideoDriver();


	/////////////////////////////////////////////////////////////////
	scene::ISceneManager* prevSmgr = device->getSceneManager()->createNewSceneManager();
	scene::IAnimatedMesh* mesh = prevSmgr->getMesh("Assets/model/ninja.b3d");
	scene::ISceneNode* node = prevSmgr->addAnimatedMeshSceneNode(mesh);
	node->setPosition(core::vector3df(0, -5, 10));
	prevSmgr->addLightSceneNode();
	prevSmgr->addCameraSceneNode();

	video::ITexture* prevTex = driver->addRenderTargetTexture(
		core::dimension2du(512,256), "prev");

	/////////////////////////////////////////////////////////////////
	scene::ISceneManager* nextSmgr = device->getSceneManager()->createNewSceneManager();
	mesh = nextSmgr->getMesh("Assets/model/reimu/reimu3.b3d");
	node = nextSmgr->addAnimatedMeshSceneNode(mesh);
	node->setPosition(core::vector3df(0, -1, 2));
	nextSmgr->addLightSceneNode()->getLightData().SpecularColor = video::SColorf();
	nextSmgr->addCameraSceneNode();

	video::ITexture* nextTex = driver->addRenderTargetTexture(
		core::dimension2du(512,256), "next");

	/////////////////////////////////////////////////////////////////
	scene::ISceneManager* mainSmgr = device->getSceneManager();
	scene::IMesh* cube = mainSmgr->getGeometryCreator()->createCubeMesh(core::vector3df(10, 6, 0.1f));

	scene::ISceneNode* front = mainSmgr->addMeshSceneNode(cube);
	front->getMaterial(0).Lighting = false;
	front->getMaterial(0).setTexture(0, prevTex);

	scene::ISceneNode* back = mainSmgr->addMeshSceneNode(cube, front);
	back->setPosition(core::vector3df(0, 0, 0.1f));
	back->getMaterial(0).Lighting = false;
	back->getMaterial(0).setTexture(0, nextTex);
	mainSmgr->addCameraSceneNode(0, core::vector3df(0, 0, -5));

	cube->drop();
	core::vector3df rot;
	while (device->run())
	{
		driver->beginScene(true, true, 0);

		//// prev
		driver->setRenderTarget(prevTex, true, true);
		driver->setViewPort(core::recti(0,0,256,128));
		prevSmgr->onAnimate();
		prevSmgr->drawAll();


// 		/// next
// 		driver->setRenderTarget(nextTex, true, true);
// 		nextSmgr->onAnimate();
// 		nextSmgr->drawAll();

		/// main
		driver->setRenderTarget(video::ERT_FRAME_BUFFER, true, true, video::SColor(255, 192, 168, 100));
		// driver->setViewPort(core::rect<s32>(128, 64, 720, 512));
		driver->setViewPort(core::recti(-128,-128,256,256));
		mainSmgr->onAnimate();
		mainSmgr->drawAll();

		driver->endScene();
	}
	
	prevSmgr->drop();
	nextSmgr->drop();
	device->drop();

	return 0;
}
