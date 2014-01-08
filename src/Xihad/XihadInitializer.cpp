#include "XihadInitializer.h"
#include "WorldRender3D\Geometry.h"
#include "irrlicht\IrrlichtDevice.h"
#include "irrlicht\ISceneManager.h"
#include "WorldRender3D\IrrlichtComponentSystemFactory.h"
#include "ScriptEngine\LuaComponentSystemFactory.h"
#include "Engine\ComponentSystemRegistry.h"

using namespace irr;
using namespace scene;
namespace xihad
{
	using namespace ngn;
	using namespace render3d;
	using namespace script;
	void initSystem( irr::IrrlichtDevice* device )
	{
		Geometry::setCreator(device->getSceneManager()->getGeometryCreator());

		auto s3d = new IrrlichtComponentSystemFactory(device);
		for (auto& clazz : s3d->inheritenceTree())
			ComponentSystemRegistry::registerSystem(clazz.typeName, s3d);

		ComponentSystemRegistry::setDefaultFactory(new LuaComponentSystemFactory(device));
	}

	void destroySystems()
	{
		ComponentSystemRegistry::destroy();
		Geometry::setCreator(nullptr);
	}

}
