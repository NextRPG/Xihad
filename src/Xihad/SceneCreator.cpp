#include "SceneCreator.h"
#include "Engine\GameScene.h"
#include <Lua\lua.hpp>
#include <iostream>
#include "luaT\stack_memo.h"
#include "ScriptEngine\LuaComponentSystem.h"

// #define ProfileForSceneLoading

using namespace std;
namespace xihad
{
	using namespace ngn;
	GameScene* createScene( const char* scriptName )
	{
		GameScene* scene = new GameScene;
		auto lcs = static_cast<script::LuaComponentSystem*>(scene->requireSystem("Lua"));
		
		lua_State* L = lcs->getLuaState();
		if (luaL_loadfile(L, scriptName) || lua_pcall(L, 0, 0, 0))
		{
			cerr << "SCRIPT ERR: " << lua_tostring(L, -1) << endl;
			lua_pop(L, 1);
			scene->destroy();
			scene = nullptr;
		}

#ifdef ProfileForSceneLoading
		scene->destroy();
		scene = 0;
#endif
		return scene;
	}

}

