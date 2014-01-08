#include "LuaComponentSystemFactory.h"
#include "LuaComponentSystem.h"
#include "Lua/lua.hpp"
#include "Engine/GameScene.h"
#include "../LuaT/stack_ops.h"
#include "../LuaT/table_ops.h"
#include "LuaScriptMacro.h"
#include "LuaT/lua_ref.h"
#include "irrlicht/IrrlichtDevice.h"

using namespace xihad::ngn;
using namespace std;
namespace xihad { namespace script 
{
	LuaComponentSystemFactory::LuaComponentSystemFactory(irr::IrrlichtDevice* dev) :
		mScriptPath("Assets/Script"), mDevice(dev)
	{
	}

	ComponentSystem* LuaComponentSystemFactory::create( GameScene* scene, const string& systemName )
	{
		static string sharedSystem = "Lua";
		if (systemName == sharedSystem)
		{
			return new LuaComponentSystem(mDevice.get(), scene, mScriptPath);
		}
		else
		{
			return scene->requireSystem(sharedSystem);
		}
	}

}}