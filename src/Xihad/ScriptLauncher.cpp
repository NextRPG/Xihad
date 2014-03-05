#include <irrlicht/irrlicht.h>
#include "XihadInitializer.h"
#include "Engine/GameScene.h"
#include "Engine/GameWorld.h"
#include "Engine/GameEngine.h"
#include "CppBase/XiAssert.h"
#include "SceneCreator.h"
#include <string>
#include "CreateDevice.h"
#include "CEGuiHandle.h"

using namespace irr;
using namespace scene;
using namespace xihad;
using namespace ngn;
int launchScript(int argc, const char** argv)
{
 	IrrlichtDevice* device = createDefaultDevice();
	initSystem(device);

	std::string path = "Assets/";
	path += argc>=2 ? argv[1] : "script/boot.lua";

	if (GameScene* scene = createScene(path.c_str()))
	{
		GameEngine* engine = new GameEngine;
		for (int i = 2; i < argc; ++i)
		{
			if (strcmp(argv[i], "-nosleep") == 0)
				engine->setNeverSleep(true);
			else if (strcmp(argv[i], "-showfps") == 0)
				engine->setShowFPS(true);
		}

		engine->getWorld()->setScene(scene);
		XiAssert::isTrue(engine->initDevice(device));
		device->drop();
		engine->launch();
		delete engine;
	}
	
	destroySystems();

	system("pause");
	return 0;

}

