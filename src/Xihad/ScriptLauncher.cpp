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
#include "Engine/FrameRateAdjuster.h"

using namespace irr;
using namespace scene;
using namespace xihad;
using namespace ngn;
int launchScript(int argc, const char** argv)
{
 	IrrlichtDevice* device = createDefaultDevice();
	IrrlichtWindow* wnd = new IrrlichtWindow(*device);
	GameEngine* engine = new GameEngine(*wnd);
	initSystem(engine);

	std::string path = "Assets/";
	path += argc>=2 ? argv[1] : "script/boot.lua";

	if (GameScene* scene = createScene(path.c_str()))
	{
		// Usually, frame rate adjust should be the last frame observer.
		// So, give it a big order
		FrameRateAdjuster* adj = new FrameRateAdjuster(1.f/60);
		engine->addFrameObserver(*adj, 1000000);
		adj->drop();

		WindowTitleUpdater* titleUpdater = new WindowTitleUpdater;
		engine->addFrameObserver(*titleUpdater);

		FPSCounter* counter = new FPSCounter;
		titleUpdater->setFPSCounter(counter);
		engine->addFrameObserver(*counter);
		counter->drop();

		if (argc>2 && strcmp(argv[2], "-showfps") == 0)
			titleUpdater->setShowFPS(true);

		titleUpdater->drop();

		WindowEventTransmitter* eventTransmitter = new WindowEventTransmitter;
		engine->addFrameObserver(*eventTransmitter, -10);
		eventTransmitter->drop();

		engine->getWorld()->setScene(scene);
		engine->launch();
		delete engine;
	}
	
	wnd->drop();
	device->drop();
	destroySystems();
	return 0;

}

