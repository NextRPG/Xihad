#include "SceneCreator.h"
#include "Engine\GameScene.h"
#include <Lua\lua.hpp>
#include <iostream>
#include "luaT\stack_memo.h"
#include "ScriptEngine\LuaComponentSystem.h"

using namespace std;
namespace xihad
{
	using namespace ngn;
	GameScene* createScene( const char* scriptName )
	{
		GameScene* scene = new GameScene;
		auto lcs = static_cast<script::LuaComponentSystem*>(scene->requireSystem("Lua"));
		
		if (!scene->requireSystem("Render"))
		{
			cerr << "Error to load Render system" << endl;
		}

		lua_State* L = lcs->getLuaState();
		if (luaL_loadfile(L, scriptName) || lua_pcall(L, 0, 0, 0))
		{
			cerr << "SCRIPT ERR: " << lua_tostring(L, -1) << endl;
			lua_pop(L, 1);
			scene->destroy();
			scene = nullptr;
		}

		return scene;
	}

}

