#include "LuaUtil.h"
#include "LuaT/luaT.h"
#include <iostream>

using namespace std;
using namespace luaT;
namespace xihad { namespace script
{
	int LuaUtil::require( lua_State* L, const char* libName )
	{
		lua_getglobal(L, "require");
		return luaT::pcall(L, 1, 0, libName);
	}

	void LuaUtil::outputErrorMessage( lua_State* L, const char* desc, int msgIdx )
	{
		cerr << "---- lua error ----\n";

		if (desc != nullptr) 
			cerr << desc << "\n";

		cerr << lua_tostring(L, msgIdx) << "\n";
		cerr << "==== lua error ====" << endl;
	}

	bool LuaUtil::iscallable( lua_State* L, int idx )
	{
		switch (lua_type(L, idx))
		{
		case LUA_TFUNCTION:
		case LUA_TTABLE:
		case LUA_TUSERDATA:
			return true;
		default:
			return false;
		};
	}

	void LuaUtil::callSelfErr( luaT::LuaRef& table, const char* funcName, bool optional )
	{
		if (!callSelf(table, funcName, optional))
		{
			outputErrorMessage(table.getState());
			lua_pop(table.getState(), 1);
		}
	}

	bool LuaUtil::callSelf( luaT::LuaRef& table, const char* funcName, bool optional /*= true*/ )
	{
		lua_State* L = table.getState();

		table.pushOnto(L);
		lua_getfield(L, -1, funcName);
		lua_remove(L, -2);	// pop table

		// if no such function then treat it as empty implmentation
		if (optional && !iscallable(L, -1))
		{
			lua_pop(L, 1); // pop non-callable value
			return true;
		}

		table.pushOnto(L);	// self call argument
		return lua_pcall(L, 1, 0, 0) == 0;
	}

	bool LuaUtil::rawCall( lua_State* L, int tidx, const char* funcName, bool optional /*= true*/ )
	{
		tidx = normalIndex(L, tidx);

		lua_pushstring(L, funcName);
		lua_rawget(L, tidx);

		// if no such function then treat it as empty implmentation
		if (optional && !iscallable(L, -1))
		{
			lua_pop(L, 1); // pop non-callable value
			return true;
		}

		return lua_pcall(L, 0, 0, 0) == 0;
	}

	std::string LuaUtil::stacktrace(lua_State* L)
	{
		StackMemo memo(L);

		lua_getglobal(L, "debug");
		if (!lua_isnil(L, -1))
		{
			lua_getfield(L, -1, "traceback");
			if (!lua_isnil(L, -1) && lua_pcall(L, 0, 1, 0) == 0)
			{
				if (const char* tracer = lua_tostring(L, -1))
					return string(tracer);
			}
		}

		return string();
	}

}}

