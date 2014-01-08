#include "metatable.h"
#include "xassert.h"
#include "stack_ops.h"

namespace luaT
{
	void Metatable::byName( lua_State* L, const char* name )
	{
		luaL_getmetatable(L, name);

		bool noMetatable = lua_isnil(L, -1);
		if (noMetatable)
			xassert(!noMetatable && 
			"Did you register your lib? No Metatable for this metatableName.");
	}

	void Metatable::bind( lua_State* L, int objIdx, const char* name )
	{
		objIdx = normalIndex(L, objIdx);
		Metatable::byName(L, name);
		bool noMetatable = lua_isnil(L, -1);
		xassert(!noMetatable && "Did you register your lib? No Metatable for this metatableName.");

		lua_setmetatable(L, objIdx);
	}

}
