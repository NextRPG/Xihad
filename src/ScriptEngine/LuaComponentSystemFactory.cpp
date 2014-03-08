#include "LuaComponentSystemFactory.h"
#include "LuaComponentSystem.h"
#include <LuaT/luaT.h>
#include "Engine/GameScene.h"
#include "LuaScriptMacro.h"
#include <irrlicht/IrrlichtDevice.h>

#include <irrlicht/ISceneManager.h>
#include <irrlicht/ISceneCollisionManager.h>
#include "WorldRender3D/IrrlichtComponentSystem.h"
#include "WorldRender3D/Geometry.h"

#include "Export/luaopen_all.h"
#include "Engine/IrrlichtWindow.h"
#include "Engine/GameEngine.h"

using namespace xihad::ngn;
using namespace xihad::render3d;
using namespace std;
using namespace luaT;
using namespace irr;
namespace xihad { namespace script 
{
	LuaComponentSystemFactory::LuaComponentSystemFactory(GameEngine* engn) :
		mScriptPath("Assets/Script"), mEngine(engn) 
	{
	}

	ComponentSystem* LuaComponentSystemFactory::create( GameScene* scene, const string& systemName )
	{
		static string sharedSystem = "Lua";
		if (systemName == sharedSystem)
		{
			initScriptEnv(scene);
			return new LuaComponentSystem(scene, mScriptPath);
		}
		else
		{
			return scene->requireSystem(sharedSystem);
		}
	}

	void LuaComponentSystemFactory::initScriptEnv(GameScene* scene)
	{
		lua_State* L = scene->getMainThread();
		NativeWindow* wnd = mEngine->getWindow();
		IrrlichtDevice* dev = (static_cast<IrrlichtWindow*>(wnd))->getIrrDevice();

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

			gui::ICursorControl* cursor = dev->getCursorControl();
			setField(L, -1, LUA_G_CURSOR, cursor);

			setField(L, -1, LUA_G_GEOMETRY, Geometry::creator());

			IrrlichtComponentSystem* irrSystem = 
				dynamic_cast<IrrlichtComponentSystem*> (scene->requireSystem("Render"));
			scene::ISceneCollisionManager* collMan = irrSystem->getSceneManager()->getSceneCollisionManager();
			setField(L, -1, LUA_G_COLLISION, collMan);

			setField(L, -1, LUA_G_ENGINE, mEngine);
			setField(L, -1, LUA_G_WINDOW, wnd);
		}
	}

}}