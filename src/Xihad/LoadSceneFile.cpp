#include <iostream>
#include <irrlicht/irrlicht.h>
#include "CreateDevice.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
int loadIrrScene(int argc, const char** argv)
{
	const char* path = //argc>1 ? argv[1] : 
		"level1/level1.irr";

	IrrlichtDevice* device = createDefaultDevice();
	ISceneManager* smgr = device->getSceneManager();
	IVideoDriver* driver = device->getVideoDriver();

	smgr->getFileSystem()->addFileArchive("level1");
	smgr->loadScene(path);
	smgr->addCameraSceneNode(0, vector3df(0, 100, 0))->setTarget(vector3df(0,0,0));
	while (device->run())
	{
		driver->beginScene(true, true, SColor(255, 144, 100, 122));
		smgr->onAnimate();
		smgr->drawAll();
		driver->endScene();
	}

	return 0;
}

