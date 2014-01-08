#include "SceneCreator.h"
#include "Engine\GameScene.h"
#include <Lua\lua.hpp>
#include <iostream>
#include "luaT\stack_memo.h"

using namespace std;
namespace xihad
{
	using namespace ngn;
	GameScene* createScene( const char* scriptName )
	{
		GameScene* scene = new GameScene;
		scene->requireSystem("Lua");
		lua_State* L = scene->mainThread();
		if (luaL_loadfile(L, scriptName) || lua_pcall(L, 0, 0, 0))
		{
			cout << "SCRIPT ERR: " << lua_tostring(L, -1) << endl;
			lua_pop(L, 1);
			scene->destroy();
			scene = nullptr;
		}

		return scene;
	}

}

