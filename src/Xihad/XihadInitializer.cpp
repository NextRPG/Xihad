#include "XihadInitializer.h"
#include "irrlicht\IrrlichtDevice.h"
#include "irrlicht\ISceneManager.h"
#include "WorldRender3D\Geometry.h"
#include "WorldRender3D\IrrlichtComponentSystemFactory.h"
#include "ScriptEngine\LuaComponentSystemFactory.h"
#include "Engine\ComponentSystemRegistry.h"
#include "Engine\GameEngine.h"
#include "Engine\IrrlichtWindow.h"
#include "AudioEngine\AudioComponentSystemFactory.h"

using namespace irr;
using namespace scene;
namespace xihad
{
	using namespace ngn;
	using namespace render3d;
	using namespace script;
	using namespace audio;

	void registerMultiSysmte(MultiComponentSystemFactory* f)
	{
		for (auto& clazz : f->inheritenceTree())
			ComponentSystemRegistry::registerSystem(clazz.typeName, f);
	}

	void initSystem( GameEngine* engine )
	{
		NativeWindow* wnd = engine->getWindow();
		IrrlichtDevice* device = (static_cast<IrrlichtWindow*>(wnd))->getIrrDevice();
		Geometry::setCreator(device->getSceneManager()->getGeometryCreator());

		registerMultiSysmte(new IrrlichtComponentSystemFactory(device));
		registerMultiSysmte(new AudioComponentSystemFactory);

		ComponentSystemRegistry::setDefaultFactory(new LuaComponentSystemFactory(engine));
	}

	void destroySystems()
	{
		ComponentSystemRegistry::destroy();
		Geometry::setCreator(nullptr);
	}

}
