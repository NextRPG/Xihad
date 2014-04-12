#pragma once
#include <Lua/lua.hpp>
#include "type_checker.h"
#include "stack_ops.h"
#include "xassert.h"

namespace luaT
{
	class LuaRef
	{
	public:
		static LuaRef fromTop(lua_State* L);

		static LuaRef fromIndex(lua_State* L, int idx)
		{
			lua_pushvalue(L, idx);
			return fromTop(L);
		}

		static LuaRef byName(lua_State* L, const char* name)
		{
			lua_getglobal(L, name);
			return fromTop(L);
		}

	public:
		LuaRef() : L(NULL), refPtr(NULL), handle(LUA_NOREF) {}

		LuaRef(const LuaRef& other);

		~LuaRef();

		LuaRef& operator=(const LuaRef& other);

		bool operator==(const LuaRef& other) const;

		void pushOnto(lua_State* otherState) const
		{
			lua_getref(otherState, handle);
		}

		void pushSelf() const
		{
			xassert(L != NULL);
			lua_getref(L, handle);
		}

		lua_State* getState() const
		{
			return L;
		}

		lua_State* getMainState() const;

	private:
		LuaRef(lua_State* L, int ref);

		void drop();

		void gain(lua_State* newState, int newHandle, int* newRefCount);

	private:
		lua_State* L;
		int* refPtr;
		int handle;
	};

	template <>
	inline void push_<LuaRef>(lua_State* L, LuaRef& ref)
	{
		ref.pushSelf();
	}

	template <>
	inline bool is_<LuaRef>(lua_State* L, int idx)
	{
		return true;
	}

	template <>
	inline LuaRef to_<LuaRef>(lua_State* L, int idx)
	{
		return LuaRef::fromIndex(L, idx);
	}

	template <>
	inline LuaRef checkarg_<LuaRef>(lua_State* L, int idx)
	{
		return LuaRef::fromIndex(L, idx);
	}
}

