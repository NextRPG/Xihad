#pragma once
#include "xassert.h"
#include "Lua/lua.hpp"

namespace luaT
{
	class StackMemo
	{
	public:
		StackMemo(lua_State* L) : L(L)
		{
			backup();
		}

		~StackMemo()
		{
			restore();
		}

		void restore()
		{
			xassert(lua_gettop(L) >= n);
			lua_settop(L, n);
		}

		void backup()
		{
			n = lua_gettop(L);
			xassert(n >= 0);
		}

	private:
		lua_State* L;
		int n;
	};
}

