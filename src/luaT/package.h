#pragma once
#include <Lua/lua.hpp>

namespace luaT
{
	struct Package
	{
		static void preload(lua_State* L, const char* name, lua_CFunction loadFunction);
		static void preload(lua_State* L, luaL_Reg* libs);
	};
}

