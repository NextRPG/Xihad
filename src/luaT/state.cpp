#include "state.h"
#include <Lua/lstate.h>

namespace luaT {
	lua_State* toMainState( lua_State* L )
	{
		return G(L)->mainthread;
	}
}
