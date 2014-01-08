#pragma once
#include "stack_ops.h"
#include "tuple.h"
#include "type_checker.h"
struct lua_State;

/************************************************************************/
/* 1. 使用引用的地方必须注意：
 *		int& xref = x;
 *		tpcall(L, 0, xref);
 *	  以上代码并不能将xref理解为int&，而仅仅是int(任何引用都会被推测为对象）。可以使用 
 *		tpcall<int&>(L, 0, xref)
 *	  则此时会显式将xref理解为引用。另外也可以使用指针取代引用，在luaT中引用和指针实现
 *	  完全相同。
 * 
/************************************************************************/
namespace luaT
{
	template <typename H, typename T>
	void popresults(lua_State* L, tuple<H, T>& results)
	{
		results.value = c_typecheck<H>(L, -ListSize<tuple<H, T> >::result);
		popresults(L, results.rests);
	}

	inline void popresults(lua_State* L, FinishType) { }

	//////////////////////////////////////////////////////////////////////////
	// Templated protected call Lua with error handler.
	//////////////////////////////////////////////////////////////////////////
#ifdef LUAT_CALL_LUA_USE_VAR_TYPE
	static inline int rtpcall_handle_impl(lua_State* L, int nresults, int errfuncIndex, int args_count)
	{
		return lua_pcall(L, args_count, nresults, errfuncIndex);
	}

	template <typename ArgHead, typename... RestArgs>
	static inline int rtpcall_handle_impl(lua_State* L, int nresults, int errfuncIndex, int args_count, ArgHead arg1, RestArgs... args)
	{
		luaT::push<ArgHead>(L, arg1);
		return rtpcall_handle_impl(L, nresults, errfuncIndex, args_count+1, args...);
	}

	template <typename... Args>
	inline int rtpcall_handle(lua_State* L, int nresults, int errfuncIndex, Args... args)
	{
		rtpcall_handle_impl(L, nresults, errfuncIndex, 0, args...);
	}


	template <typename... Args>
	inline int call(lua_State* L, int nresults, Args... args)
	{
		rtpcall_handle_impl(L, nresults, 0, 0, args...);
	}

#else	// C++11 Variable Length Template Feature Not Supported.

	inline int pcall(lua_State* L, int nresults, int errfuncIndex)
	{ 
		return lua_pcall(L, 0, nresults, errfuncIndex);
	}

	template <typename A1>
	inline int pcall(lua_State* L, int nresults, int errfuncIndex, A1 arg1)
	{
		errfuncIndex = normalIndex(L, errfuncIndex);
		push<A1>(L, arg1);
		return lua_pcall(L, 1, nresults, errfuncIndex);
	}

	template <typename A1, typename A2>
	inline int pcall(lua_State* L, int nresults, int errfuncIndex, A1 arg1, A2 arg2)
	{
		errfuncIndex = normalIndex(L, errfuncIndex);
		push<A1, A2>(L, arg1, arg2);
		return lua_pcall(L, 2, nresults, errfuncIndex);
	}

	template <typename A1, typename A2, typename A3>
	inline int pcall(lua_State* L, int nresults, int errfuncIndex, A1 arg1, A2 arg2, A3 arg3)
	{
		errfuncIndex = normalIndex(L, errfuncIndex);
		push<A1, A2, A3>(L, arg1, arg2, arg3);
		return lua_pcall(L, 3, nresults, errfuncIndex);
	}

	template <typename A1, typename A2, typename A3, typename A4>
	inline int pcall(lua_State* L, int nresults, int errfuncIndex, A1 arg1, A2 arg2, A3 arg3, A4 arg4)
	{
		errfuncIndex = normalIndex(L, errfuncIndex);
		push<A1, A2, A3, A4>(L, arg1, arg2, arg3, arg4);
		return lua_pcall(L, 4, nresults, errfuncIndex);
	}

	template <typename A1, typename A2, typename A3, typename A4, typename A5>
	inline int pcall(lua_State* L, int nresults, int errfuncIndex, A1 arg1, A2 arg2, A3 arg3, A4 arg4, A5 arg5)
	{
		errfuncIndex = normalIndex(L, errfuncIndex);
		push<A1, A2, A3, A4, A5>(L, arg1, arg2, arg3, arg4, arg5);
		return lua_pcall(L, 5, nresults, errfuncIndex);
	}

	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	inline int pcall(lua_State* L, int nresults, int errfuncIndex, A1 arg1, A2 arg2, A3 arg3, A4 arg4, A5 arg5, A6 arg6)
	{
		errfuncIndex = normalIndex(L, errfuncIndex);
		push<A1, A2, A3, A4, A5, A6>(L, arg1, arg2, arg3, arg4, arg5, arg6);
		return lua_pcall(L, 6, nresults, errfuncIndex);
	}


	//////////////////////////////////////////////////////////////////////////
	// Generic C call Lua, without error handler.
	//////////////////////////////////////////////////////////////////////////
	inline void call(lua_State* L, int nresults) 
	{ 
		lua_call(L, 0, nresults);
	}

	template <typename A1>
	inline void call(lua_State* L, int nresults, A1 arg1)
	{
		push<A1>(L, arg1);
		lua_call(L, 1, nresults);
	}

	template <typename A1, typename A2>
	inline void call(lua_State* L, int nresults, A1 arg1, A2 arg2)
	{
		push<A1, A2>(L, arg1, arg2);
		lua_call(L, 2, nresults);
	}

	template <typename A1, typename A2, typename A3>
	inline void call(lua_State* L, int nresults, A1 arg1, A2 arg2, A3 arg3)
	{
		push<A1, A2, A3>(L, arg1, arg2, arg3);
		lua_call(L, 3, nresults);
	}

	template <typename A1, typename A2, typename A3, typename A4>
	inline void call(lua_State* L, int nresults, A1 arg1, A2 arg2, A3 arg3, A4 arg4)
	{
		push<A1, A2, A3, A4>(L, arg1, arg2, arg3, arg4);
		lua_call(L, 4, nresults);
	}

	template <typename A1, typename A2, typename A3, typename A4, typename A5>
	inline void call(lua_State* L, int nresults, A1 arg1, A2 arg2, A3 arg3, A4 arg4, A5 arg5)
	{
		push<A1, A2, A3, A4, A5>(L, arg1, arg2, arg3, arg4, arg5);
		lua_call(L, 5, nresults);
	}

	template <typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
	inline void call(lua_State* L, int nresults, A1 arg1, A2 arg2, A3 arg3, A4 arg4, A5 arg5, A6 arg6)
	{ 
		push<A1, A2, A3, A4, A5, A6>(L, arg1, arg2, arg3, arg4, arg5, arg6);
		lua_call(L, 6, nresults);
	}

#endif LUAT_CALL_LUA_USE_VAR_TYPE
}

