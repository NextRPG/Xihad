#include "LuaProperties.h"
#include "luaT\stack_memo.h"

namespace xihad { namespace ngn
{
	LuaProperties::LuaProperties( const luaT::LuaRef& tableRef ) : 
		lobject(tableRef)
	{
	}

	Properties::B LuaProperties::getBool( S key, B defaultValue ) const
	{
		lua_State* L = lobject.getState();
		luaT::StackMemo memo(L);

		lobject.pushSelf();
		lua_getfield(L, -1, key);
		if (lua_isboolean(L, -1))
			return lua_toboolean(L, -1) == 1;
		
		return defaultValue;
	}

	Properties::I LuaProperties::getInt( S key, I defaultValue ) const
	{
		lua_State* L = lobject.getState();
		luaT::StackMemo memo(L);

		lobject.pushSelf();
		lua_getfield(L, -1, key);
		if (lua_isnumber(L, -1))
			return lua_tointeger(L, -1);
		
		return defaultValue;
	}

	Properties::F LuaProperties::getFloat( S key, F defaultValue ) const
	{
		lua_State* L = lobject.getState();
		luaT::StackMemo memo(L);

		lobject.pushSelf();
		lua_getfield(L, -1, key);
		if (lua_isnumber(L, -1))
			return (F) lua_tonumber(L, -1);
		
		return defaultValue;
	}

	Properties::D LuaProperties::getDimen( S key, D defaultValue ) const
	{
		lua_State* L = lobject.getState();
		luaT::StackMemo memo(L);

		lobject.pushSelf();
		lua_getfield(L, -1, key);
		if (lua_istable(L, -1))
		{
			lua_rawgeti(L, -1, 1);
			lua_rawgeti(L, -2, 2);
			if (lua_isnumber(L, -1) && lua_isnumber(L, -2))
			{
				return D((float) lua_tonumber(L, -2), 
						 (float) lua_tonumber(L, -1));
			}
		}

		return defaultValue;
	}

	Properties::V LuaProperties::getVector( S key, V defaultValue ) const
	{
		lua_State* L = lobject.getState();
		luaT::StackMemo memo(L);

		lobject.pushSelf();
		lua_getfield(L, -1, key);
		if (lua_istable(L, -1))
		{
			lua_rawgeti(L, -1, 1);
			lua_rawgeti(L, -2, 2);
			lua_rawgeti(L, -3, 3);
			if (lua_isnumber(L, -1) && lua_isnumber(L, -2) && lua_isnumber(L, -3))
			{
				return V((float) lua_tonumber(L, -3), 
						 (float) lua_tonumber(L, -2), 
						 (float) lua_tonumber(L, -1));
			}
		}

		return defaultValue;
	}

	Properties::Q LuaProperties::getQuater( S key, Q defaultValue ) const
	{
		lua_State* L = lobject.getState();
		luaT::StackMemo memo(L);

		lobject.pushSelf();
		lua_getfield(L, -1, key);
		if (lua_istable(L, -1))
		{
			lua_rawgeti(L, -1, 1);
			lua_rawgeti(L, -2, 2);
			lua_rawgeti(L, -3, 3);
			lua_rawgeti(L, -4, 4);
			if (lua_isnumber(L, -1) && lua_isnumber(L, -2) && 
				lua_isnumber(L, -3) && lua_isnumber(L, -4))
			{
				return Q((float) lua_tonumber(L, -4), 
					     (float) lua_tonumber(L, -3), 
					     (float) lua_tonumber(L, -2),
						 (float) lua_tonumber(L, -1));
			}
		}

		return defaultValue;
	}

	Properties::S LuaProperties::getString( S key, S defaultValue ) const
	{
		lua_State* L = lobject.getState();
		luaT::StackMemo memo(L);

		lobject.pushSelf();
		lua_getfield(L, -1, key);
		if (lua_isstring(L, -1))
			return lua_tostring(L, -1);

		return defaultValue;
	}

	void LuaProperties::pushSelf( lua_State* L ) const
	{
		lobject.pushOnto(L);
	}

}}

