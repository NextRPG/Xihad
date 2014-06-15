#pragma once

struct lua_State;

namespace luaT {
	lua_State* toMainState(lua_State* L);
}
