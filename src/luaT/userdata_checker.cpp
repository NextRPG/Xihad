#include "userdata_checker.h"
#include "reserved.h"

namespace luaT
{
	void* UserdataTypeCheckerNT::castUserdata( lua_State* L, int idx)
	{
		if (lua_istable(L, idx))
		{	// table.
			lua_getfield(L, idx, ReservedKeyword::__UDKEY);

			void* userdata = lua_islightuserdata(L, -1) ? lua_touserdata(L, -1) : 0;
			lua_pop(L, 1);

			return userdata;
		}

		if (lua_isuserdata(L, idx))
			return lua_touserdata(L, idx);

		return 0;
	}

	void* UserdataTypeCheckerNT::shiftUserdata( lua_State* L, void* userdata, int targetMtIndex )
	{
		assert(targetMtIndex > 0);

		char* in_ud = (char*) (userdata);
		// check whether this is corresponding type or derived type.
		while(!lua_equal(L, -1, targetMtIndex))
		{
			if (!lua_getmetatable(L, -1))
				return NULL;	// early jump out

			// shift userdata caused by multi-inherit
			lua_pushstring(L, ReservedKeyword::__OFFSET);
			lua_rawget(L, -3);	// mt.offset
			in_ud += luaL_optinteger(L, -1, 0);
			lua_pop(L, 1);		// pop offset
		}

		return in_ud;
	}

	static int pushTargets(lua_State* L, int idx, void* ud, const char* mtName)
	{
		int pushCount = 0;
		if (ud == NULL) return pushCount;

		idx = normalIndex(L, idx);
		Metatable::byName(L, mtName);
		++pushCount;

		if (lua_getmetatable(L, idx)) 
			++pushCount;

		return pushCount;
	}

#ifdef LUAT_OPTIMIZE_NON_POLYMORPHIC
	void* PolyClassChecker::noShiftChecking(lua_State* L, int idx, void* ud, const char* mtName)
	{
		int pushCount = pushTargets(L, idx, ud, mtName);
		idx = idx>0 ? idx : (idx-pushCount);	// 不支持伪索引
		
		if (pushCount != 2 || !lua_equal(L, -1, -2))
			ud = NULL;

		lua_pop(L, pushCount);
		return ud;
	}
#endif

	void* PolyClassChecker::shiftOnChecking(lua_State* L, int idx, void* ud, const char* mtName)
	{
		int pushCount = pushTargets(L, idx, ud, mtName);
		int targetMtIdx = lua_gettop(L) - pushCount + 1;

		if (pushCount == 2)
			ud = UserdataTypeCheckerNT::shiftUserdata(L, ud, targetMtIdx); // correct target metatable index to positive index
		else
			ud = NULL;

		lua_settop(L, targetMtIdx-1);
		return ud;
	}
}

