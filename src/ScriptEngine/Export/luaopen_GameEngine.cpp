#include <Engine/GameEngine.h>
#include <luaT/luaT.h>

using namespace luaT;
using namespace xihad::ngn;
namespace xihad { namespace script
{
	int luaopen_GameEngine(lua_State* L)
	{
		luaT_defRegsBgn(engine)
			luaT_mnamedfunc(GameEngine, getWindow),
			luaT_mnamedfunc(GameEngine, isRunning),
			luaT_mnamedfunc(GameEngine, stop),
		luaT_defRegsEnd
		MetatableFactory<GameEngine>::create(L, engine);
		return 0;
	}

}}

