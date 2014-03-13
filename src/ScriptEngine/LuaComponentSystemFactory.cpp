#include "LuaComponentSystemFactory.h"
#include "LuaComponentSystem.h"
#include <LuaT/luaT.h>
#include "Engine/GameScene.h"
#include "LuaScriptMacro.h"

#include "Export/luaopen_all.h"
#include "Engine/GameEngine.h"

using namespace xihad::ngn;
using namespace std;
using namespace luaT;
using namespace irr;
namespace xihad { namespace script 
{
	LuaComponentSystemFactory::LuaComponentSystemFactory(GameEngine* engn) :
		mEngine(engn) 
	{
	}

	ComponentSystem* LuaComponentSystemFactory::create( GameScene* scene, const string& systemName )
	{
		static string sharedSystem = "Lua";
		if (systemName == sharedSystem)
		{
			LuaComponentSystem* lcs = new LuaComponentSystem(scene);
			initScriptEnv(scene, lcs->getLuaState());
			return lcs;
		}
		else
		{
			return scene->requireSystem(sharedSystem);
		}
	}

	void LuaComponentSystemFactory::initScriptEnv(GameScene* scene, lua_State* L)
	{
		NativeWindow* wnd = mEngine->getWindow();

		{
			StackMemo memo(L);
			// export cpp library to lua
			luaopen_all(L);
		}

		// set global environment
		{
			StackMemo memo(L);
			// set global variables
			lua_getglobal(L, "_G");

			// _G.scene 
			setField(L, -1, LUA_G_SCENE, scene);

			// _G.Time
			lua_createtable(L, 0, 3);
			{
				// _G.Time.change
				setField(L, -1, LUA_G_TIME_CHANGE, 0);
				// _G.Time.global
				setField(L, -1, LUA_G_TIME_GLOBAL, 0);
			}
			lua_setfield(L, -2, LUA_G_TIME);	// TODO: read-only?

			setField(L, -1, LUA_G_ENGINE, mEngine);
			setField(L, -1, LUA_G_WORLD, mEngine->getWorld());
			setField(L, -1, LUA_G_WINDOW, wnd);
		}
	}

}}