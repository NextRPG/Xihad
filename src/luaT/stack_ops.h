#pragma once

extern "C" {
#include <Lua/lua.h>
}

#include "param_types.h"
#include "type_traits.h"
#include "new_userdata.h"

namespace luaT
{

	inline int normalIndex(lua_State* L, int idx)
	{
		if (idx < 0)
		{
			int top = lua_gettop(L);
			if (-idx <= top)
			{
				idx = top + idx + 1;
			}
		}
		return idx;
	}

	//=======================================================================
	// Generic Push Function. It's useful to implement generic cpp call lua 
	// function
	//=======================================================================
	template <typename T>
	inline void push(lua_State* L, typename TypeTraits<T>::ParameterType value)
	{
		typedef typename TypeTraits<T>::NoConstType NoConstT;

		push_<NoConstT>(L, (typename TypeTraits<NoConstT>::ParameterType) value);
	}
	/************************************************************************/
	/* 默认实现。
	 * 如果要求压栈一个指针对象。
	 * 那么被理解成，压栈一个 userdata，此 userdata 为 T**。
	 * 如果要这么做，必须得注册 T* 的 metatable，并提供metatablename实现.
	/************************************************************************/
	template <typename T>
	inline void push_(lua_State* L, typename TypeTraits<T>::ParameterType value)
	{
		static_assert(sizeof(T), "T.h is needed if T is not pointer");

		newUserdata<T>(L, value);
	}

	template <>
	inline void push_<Nil>(lua_State* L, Nil& type)
	{
		(void) type;
		lua_pushnil(L);
	}

	template <>
	inline void push_<Thread>(lua_State* L, Thread& type)
	{
		(void) type;
		lua_pushthread(L);
	}

	template <>
	inline void push_<double>(lua_State* L, double pNumber)
	{	
		lua_pushnumber(L, pNumber);
	}

	template <>
	inline void push_<float>(lua_State* L, float pNumber)
	{	
		lua_pushnumber(L, pNumber);
	}

	template <>
	inline void push_<int>(lua_State* L, int pValue)
	{
		lua_pushinteger(L, pValue);
	}

	template <>
	inline void push_<unsigned>(lua_State* L, unsigned pValue)
	{
		lua_pushnumber(L, pValue);
	}

	template <>
	inline void push_<LString>(lua_State* L, LString& pStr)
	{
		if (pStr.size == 0)
		{
			lua_pushstring(L, pStr.value);
		}
		else
		{
			lua_pushlstring(L, pStr.value, pStr.size);
		}
	}

	template <>
	inline void push_<char*>(lua_State* L, char* cstr)
	{
		lua_pushstring(L, cstr);
	}

	template <>
	inline void push_<bool>(lua_State* L, bool pValue)
	{
		lua_pushboolean(L, pValue);
	}

	template <>
	inline void push_<CClosure>(lua_State* L, CClosure& closure)
	{
		lua_pushcclosure(L, closure.func, closure.n);
	}

	template <>
	inline void push_<lua_CFunction>(lua_State* L, lua_CFunction cfunction)
	{
		lua_pushcfunction(L, cfunction);
	}

	template <typename A1, typename A2>
	inline void push(lua_State* L, typename TypeTraits<A1>::ParameterType arg1, typename TypeTraits<A2>::ParameterType arg2)
	{ 
		push<A1>(L, arg1); 
		push<A2>(L, arg2); 
	}

	template <typename A1, typename A2, typename A3>
	inline void push(lua_State* L, typename TypeTraits<A1>::ParameterType arg1, typename TypeTraits<A2>::ParameterType arg2, typename TypeTraits<A3>::ParameterType arg3)
	{ 
		push<A1, A2>(L, arg1, arg2); 
		push<A3>(L, arg3);
	}

	template <typename A1, typename A2, typename A3, typename A4>
	inline void push(lua_State* L, typename TypeTraits<A1>::ParameterType arg1, typename TypeTraits<A2>::ParameterType arg2, typename TypeTraits<A3>::ParameterType arg3, typename TypeTraits<A4>::ParameterType arg4)
	{ 
		push<A1, A2, A3>(L, arg1, arg2, arg3);
		push<A4>(L, arg4);
	}

	template <typename A1, typename A2, typename A3, typename A4, typename A5>
	inline void push(lua_State* L, typename TypeTraits<A1>::ParameterType arg1, typename TypeTraits<A2>::ParameterType arg2, typename TypeTraits<A3>::ParameterType arg3, typename TypeTraits<A4>::ParameterType arg4, typename TypeTraits<A5>::ParameterType arg5)
	{
		push<A1, A2, A3, A4>(L, arg1, arg2, arg3, arg4);
		push<A5>(L, arg5);
	}

	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	inline void push(lua_State* L, typename TypeTraits<A1>::ParameterType arg1, typename TypeTraits<A2>::ParameterType arg2, typename TypeTraits<A3>::ParameterType arg3, typename TypeTraits<A4>::ParameterType arg4, typename TypeTraits<A5>::ParameterType arg5, typename TypeTraits<A6>::ParameterType arg6)
	{
		push<A1, A2, A3, A4, A5>(L, arg1, arg2, arg3, arg4, arg5);
		push<A6>(L, arg6);
	}

	template <class Writer>
	void stackDump(lua_State* L, Writer& writer, bool top2Bottom = true, const char* split = "\n")
	{
		char buf[9];
		int top = lua_gettop(L);

		int begin = top2Bottom ? top : 1;
		int diff = top2Bottom ? -1 : 1;
		int end = top + 1 - begin + diff;

		for (; begin != end; begin += diff)
		{
			int typecode = lua_type(L, begin);

			sprintf_s(buf, 9, "% 8s", lua_typename(L, typecode));
			writer <<  buf << ": ";

			switch (typecode)
			{
			case LUA_TNUMBER:
				writer << lua_tonumber(L, begin);
				break;
			case LUA_TSTRING:
				writer << lua_tostring(L, begin);
				break;
			case LUA_TBOOLEAN:
				writer << lua_toboolean(L, begin) ? "true" : "false";
				break;
			case LUA_TNIL:
				writer << "nil";
				break;
			default:
				writer << lua_topointer(L, begin);
				break;
			}

			writer << split;
		}
	}
}