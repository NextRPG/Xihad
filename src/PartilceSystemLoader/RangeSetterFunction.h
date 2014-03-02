#pragma once
#include <lua/lua.hpp>
#include <string>

namespace xihad { namespace particle
{
	template <const char* const rangeField>
	int rangeSetter(lua_State* L)
	{
		int maxIndex = 3;
		if (lua_gettop(L) == 2)
			maxIndex = 2;

		std::string setMin = "setMin";
		setMin += rangeField;
		lua_getfield(L, 1, setMin.c_str());
		lua_pushvalue(L, 1);
		lua_pushvalue(L, 2);
		lua_call(L, 2, 0);

		std::string setMax = "setMax";
		setMax += rangeField;
		lua_getfield(L, 1, setMax.c_str());
		lua_pushvalue(L, 1);
		lua_pushvalue(L, maxIndex);
		lua_call(L, 2, 0);

		return 0;
	}


}}

