#pragma once

#include "Lua/lua.hpp"
#include "metatable_data.h"

//////////////////////////////////////////////////////////////////////////
//   API For Export Your Existing C/C++ Function or C++ class To Lua    //
//////////////////////////////////////////////////////////////////////////

namespace luaT {

	struct Metatable
	{
		// push metatable for type onto stack
		template <typename T>
		static inline void forType(lua_State* L)
		{
			byName(L, MetatableData<T>::name());
		}
		
		template <typename T>
		static inline void bind(lua_State* L, int objIdx)
		{
			bind(L, objIdx, MetatableData<T>::name());
		}

		// push metatable with the specified name onto stack
		static void byName(lua_State* L, const char* name);

		static void bind(lua_State* L, int objIdx, const char* name);
	};

}


