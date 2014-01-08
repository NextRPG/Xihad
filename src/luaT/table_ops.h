#pragma once

#include "stack_ops.h"

namespace luaT
{
	//================================
	// General Table Operation.
	//================================
	template <typename K, typename V>
	inline void setGlobal(lua_State* L, K pKey, V pValue)
	{
		setField(L, LUA_GLOBALSINDEX, pKey, pValue);
	}

	template <typename K, typename V>
	inline void setField(lua_State* L, int tableIndex, K pKey, V pValue)
	{
		tableIndex = normalIndex(L, tableIndex);
		push<K>(L, pKey);
		push<V>(L, pValue);
		lua_settable(L, tableIndex);
	}

	template <typename K, typename V>
	inline void rawsetField(lua_State* L, int tableIndex, K pKey, V pValue)
	{
		tableIndex = normalIndex(L, tableIndex);
		push<K>(L, pKey);
		push<V>(L, pValue);
		lua_rawset(L, tableIndex);
	}

	template <typename K>
	inline void getField(lua_State* L, int tableIndex, K pKey)
	{
		tableIndex = normalIndex(L, tableIndex);
		push<K>(L, pKey);
		lua_gettable(L, tableIndex);
	}

	template <typename K>
	inline void rawGetField(lua_State* L, int tableIndex, K pKey)
	{
		tableIndex = normalIndex(L, tableIndex);
		push<K>(L, pKey);
		lua_rawget(L, tableIndex);
	}

}