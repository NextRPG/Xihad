#include "state.h"
#include <Lua/lstate.h>

lua_State* toMainState( lua_State* L )
{
	return G(L)->mainthread;
}
