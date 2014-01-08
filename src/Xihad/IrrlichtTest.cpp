#include <irrlicht/irrlicht.h>
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

	IBillboardSceneNode* bill = smgr->addBillboardSceneNode();
	bill->setMaterialType(video::EMT_TRANSPARENT_ADD_COLOR);
	bill->setMaterialTexture(0, driver->getTexture("Assets/gfx/fire.bmp"));
	bill->setMaterialFlag(video::EMF_LIGHTING, false);
	bill->setMaterialFlag(video::EMF_ZBUFFER, false);
	bill->setSize(10, 10, 10);
	bill->setRotation(core::vector3df(30, 45, 60));

	s32 lastFPS = -1;
	while (device->run())
	{
		driver->beginScene(true, true, SColor(255, 144, 100, 122));

		smgr->drawAll();

		driver->endScene();

		const s32 fps = driver->getFPS();
		if (lastFPS != fps)
		{
			core::stringw str = L"Irrlicht Engine - SpecialFX example [";
			str += driver->getName();
			str += "] FPS:";
			str += fps;

			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}
	}

	return 0;
}
