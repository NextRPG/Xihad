#include <luaT/luaT.h>
#include <Engine/GameWorld.h>

using namespace luaT;
namespace xihad { namespace script
{
	using namespace ngn;

	int luaopen_GameWorld(lua_State* L)
	{
		luaT_defRegsBgn(world)
			luaT_mnamedfunc(GameWorld, setPaused),
			luaT_mnamedfunc(GameWorld, isPaused),
			luaT_mnamedfunc(GameWorld, setTimeScale),
			luaT_mnamedfunc(GameWorld, getTimeScale),
			luaT_mnamedfunc(GameWorld, singleStep),
			luaT_mnamedfunc(GameWorld, setSingleStepSeconds),
			luaT_mnamedfunc(GameWorld, getSingleStepSeconds),
		luaT_defRegsEnd
		MetatableFactory<GameWorld>::create(L, world);

		return 0;
	}
}}

