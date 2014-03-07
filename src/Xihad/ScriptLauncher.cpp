#include <string>
#include "XihadInitializer.h"
#include "Engine/GameScene.h"
#include "Engine/GameWorld.h"
#include "Engine/GameEngine.h"
#include "CppBase/XiAssert.h"
#include "SceneCreator.h"
#include "CreateDevice.h"
#include "irrlicht/IrrlichtDevice.h"
#include "Engine/IrrlichtWindow.h"
#include "Engine/FPSCounter.h"
#include "Engine/WindowTitleUpdater.h"
#include "Engine/WindowEventTransmitter.h"

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
		NativeWindow* wnd = new IrrlichtWindow(*device);
		GameEngine* engine = new GameEngine(*wnd);
		wnd->drop();

		WindowTitleUpdater* titleUpdater = new WindowTitleUpdater;
		if (argc>2 && strcmp(argv[2], "-showfps") == 0)
		{
			FPSCounter* counter = new FPSCounter;
			titleUpdater->setFPSCounter(counter);
			engine->addFrameObserver(*counter);
			counter->drop();
		}
		engine->addFrameObserver(*titleUpdater);
		titleUpdater->drop();

		WindowEventTransmitter* eventTransmitter = new WindowEventTransmitter;
		engine->addFrameObserver(*eventTransmitter);
		eventTransmitter->drop();

		engine->getWorld()->setScene(scene);
		device->drop();
		engine->launch();
		delete engine;
	}
	
	destroySystems();

	system("pause");
	return 0;

}

